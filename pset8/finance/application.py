import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from operator import itemgetter

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT * FROM stocks WHERE user_id=? ORDER BY symbol", session["user_id"])
    # stocks is a list of dictionaries, where stock is each individual dictionary:
    # using a for loop add new key-value pair to each stock dictionary which contains the current price and name of the stock symbol
    # calculate current total cost of stock (shares * price)
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["name"] = lookup(stock["symbol"])["name"]
        stock["total"] = stock["price"] * stock["shares"]
    # get the amount of cash the user currentle has
    cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
    # initialize money variable which will display cash + value of all stocks and calculate it
    money = cash
    for stock in stocks:
        money += stock["total"]
    return render_template("index.html", stocks=stocks, cash=cash, money=money)


@app.route("/quote", methods=["GET", "POST"])
def quote():
    """Quote the price of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Quote the API for stock's price
        result = lookup(request.form.get("symbol"))
        if result == None:
            return apology("invalid symbol")
        else:
            return render_template("quoted.html", name=result["name"], symbol=result["symbol"], price=result["price"])
    else:
    # User reached route via GET (as by clicking a link or via redirect)
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        #       --- ERROR CHECKING ---
        # Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("Invalid symbol")
        # Ensure the symbol is valid stock symbol
        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid symbol")
        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("Invalid number")
        # Ensure that number of shares is an integer
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid number")
        shares = int(request.form.get("shares"))
        # Ensure that shares is a positive int
        if shares <= 0:
            return apology("Invalid number")
        price = lookup(request.form.get("symbol"))["price"]
        symbol = lookup(request.form.get("symbol"))["symbol"]
        total = price * shares
        # check how much cash does the user have
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        # Ensure that user have enough money to buy stocks
        if (cash - total) < 0:
            return apology("can't afford :/")
        # The values submitted are valid, user has enough money =>
        #       --- LOGIC FOR BUYING STOCKS ---
        else:
            # if user has such stock:
            if len(db.execute("SELECT symbol FROM stocks WHERE symbol=? AND user_id=?",
                              symbol, session["user_id"])) == 1:
                # new number of shares for user = shares from db + shares from from
                user_shares = shares + db.execute("SELECT shares FROM stocks WHERE symbol=? AND user_id=?",
                                     symbol, session["user_id"])[0]["shares"]
                # update the stock table by saving the new num of shares for the stock they bought
                db.execute("UPDATE stocks SET shares=? WHERE symbol=? AND user_id=?",
                           user_shares, symbol, session["user_id"])
            # if user does not have such stock
            else:
                # insert to the stocks table new row: symbol, user_id, shares (who bought, what bought and how much)
                db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)",
                           session["user_id"], symbol, shares)
            # update amount of cash in the users table
            cash -= total
            db.execute("UPDATE users SET cash=? WHERE id=?", cash, session["user_id"])
            # save the transaction to history table
            db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                       session["user_id"], symbol, shares, price)
        # redirect to index
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, price, datetime(timestamp, 'localtime') as timestamp FROM history WHERE user_id=? ORDER BY timestamp DESC",
                              session["user_id"])
    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # --- ERROR CHECKING ---
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        # Ensure password confirmation was submitted
        elif not request.form.get("confirm-password"):
            return apology("must confirm password", 403)
        # Ensure values of password and confirmation password fields match
        elif request.form.get("password") != request.form.get("confirm-password"):
            return apology("passwords must match", 403)

        # Hash new user's password
        hashp = generate_password_hash(request.form.get("password"))
        # Query db for username
        username = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(username) == 1:
            # if it exists - error
            return apology("username already exists", 403)
        else:
            # if not  - store username and hash in the db
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hashp)
        # Redirect user to home page
        return redirect("/")
    else:
    # User reached route via GET (as by clicking a link or via redirect)
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_stock = db.execute("SELECT symbol FROM stocks WHERE user_id=? ORDER BY symbol", session["user_id"])
    # store user stocks in SYMBOLS list in order to check that the value submitted is really one of the stocks the user has
    SYMBOLS = []
    for stock in user_stock:
        SYMBOLS.append(stock["symbol"])
    # User reached route via POST (as by submitting a form via POST)
    #       --- ERROR CHECKING ---
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("No symbol")
        elif request.form.get("symbol") not in SYMBOLS:
            return apology("Invalid symbol")
        elif not request.form.get("shares"):
            return apology("Invalid number")
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid number")
        shares = int(request.form.get("shares"))
        # Ensure that amount is a positive int
        if shares < 0:
            return apology("Invalid number")
        symbol = lookup(request.form.get("symbol"))["symbol"]
        price = lookup(symbol)["price"]
        # Ensure that the user has enough shares to sell
        user_shares = db.execute("SELECT shares FROM stocks WHERE symbol=? AND user_id=?",
                                 symbol, session["user_id"])[0]["shares"]
        if user_shares - shares < 0:
            return apology("Not enough shares to sell")
        #       --- LOGIC FOR SELLING STOCK ---
        total = price * shares
        # update amount of cash in the users table (add total to cash)
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        cash += total
        db.execute("UPDATE users SET cash=? WHERE id=?", cash, session["user_id"])
        # update shares amount the the user has after selling stock or delete if he has none left
        user_shares -= shares
        if user_shares == 0:
             db.execute("DELETE FROM stocks WHERE symbol=? AND user_id=?",
                        symbol, session["user_id"])
        else:
            db.execute("UPDATE stocks SET shares=? WHERE symbol=? AND user_id=?",
                       user_shares, symbol, session["user_id"])
        # save the transaction to history table
        # shares i going to be a negative int because it is "sell" transaction
        shares = 0 - shares
        db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, shares, price)
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", user_stock=user_stock)


@app.route("/changepass", methods=["GET", "POST"])
@login_required
def changepass():
    """Allow user to change their password"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # --- ERROR CHECKING ---
        # Ensure old password was submitted
        if not request.form.get("oldpass"):
            return apology("No old password")
        # Ensure new password was submitted
        elif not request.form.get("newpass"):
            return apology("Must provide new password")
        # Ensure password was repeated
        elif not request.form.get("repeatpass"):
            return apology("Must repeat password")
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id=?", session["user_id"])
        # # Ensure old password is valid
        if not check_password_hash(rows[0]["hash"], request.form.get("oldpass")):
            return apology("Invalid old password")
        # Ensure that new password is at least 6 characters long
        elif len(request.form.get("newpass")) < 6 or len(request.form.get("newpass")) > 12:
            return apology("New password must be between 6 and 12 characters")
        # Ensure values of new password and repeat password fields match
        elif request.form.get("newpass") != request.form.get("repeatpass"):
            return apology("Passwords must match")
        # Hash new password
        hashp = generate_password_hash(request.form.get("newpass"))
        # Save new password into the users table
        db.execute("UPDATE users SET hash=? WHERE id=?", hashp, session["user_id"])
        # Redirect user to home page
        return redirect("/")
    else:
    # User reached route via GET (as by clicking a link or via redirect)
        return render_template("changepass.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
