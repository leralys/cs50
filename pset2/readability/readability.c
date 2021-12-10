;l
float letters(string str), words(string str), sentences(string str);
float average(float x, float y), L, S;
void index(float l, float s);
int num_letters = 0;
int num_words = 0;
int num_sentences = 0;
bool start = false;

int main(void)
{
    //prompt a user for text input
    string text = get_string("Text: ");

    //calculate the number of letters, words and sentences
    letters(text);
    words(text);
    sentences(text);

    //calculate the average num of  letters and sentences per 100 words
    L = average(num_letters, num_words);
    S = average(num_sentences, num_words);

    //calculate the Coleman-Liau index 0.0588 * L - 0.296 * S - 15.8
    index(L, S);
}

float letters(string str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        //when see an alphabetic character increase the num of letters by 1
        if (isalpha(str[i]) > 0)
        {
            num_letters ++;
        }
    }
    return num_letters;
}

float words(string str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        //look for the first alphabetic character and increase the num of words by 1
        if (start == false)
        {
            if (isalpha(str[i]) > 0)
            {
                num_words ++;
                start = true;
            }
        }
        //afterwards don't run the previous code, look for spaces and increase the num of words by 1
        else
        {
            if (isspace(str[i]) > 0)
            {
                num_words ++;
            }
        }
    }
    return num_words;
}

float sentences(string str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        //when see the end of sentence (indicated by !, ?, or .) increase the num of sentences by 1
        if (str[i] == '!' || str[i] == '?' || str[i] == '.')
        {
            num_sentences ++;
        }
    }
    return num_sentences;
}


float average(float x, float y)
// x = letters or sentences
// y = words
{
    return (x * 100) / y;
}

void index(float l, float s)
{
    float i = round((0.0588 * l) - (0.296 * s) - 15.8);
    //if the index is less than 1, print "Before Grade 1"
    if (i < 1)
    {
        printf("Before Grade 1\n");
    }
    //if the index is 16 or higher print Grade 16+
    else if (i >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", i);
    }
}
