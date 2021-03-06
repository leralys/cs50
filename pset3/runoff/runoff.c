#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //iterate over the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //check if string name is a name of any candidate
        if (strcmp(name, candidates[i].name) == 0)
            //if yes - update the preferences array - [voter][rank] - with the index of the name
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    //if name is not a name of any candidate
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    //loop over the voters - v for voter
    for (int v = 0; v < voter_count; v++)
    {
        //loop over the ranks - r for rank
        for (int r = 0; r < candidate_count; r++)
        {
            //check if the candidate (whose index is stored in the preferences[voter][rank]) is eliminated
            if (!candidates[preferences[v][r]].eliminated)
            {
                //if it is not eliminated - update the votes of that candidate and exit the loop
                candidates[preferences[v][r]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //loop over the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if the number of votes of a candidate is more than half of total number of votes
        if (candidates[i].votes > voter_count / 2)
        {
            //print the name of the winner and return true
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    //otherwise
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min_vote;
    //loop over the candidates to "read" them
    for (int i = 0; i < candidate_count; i++)
    {
        //check if the candidate is still in the election
        if (!candidates[i].eliminated)
        {
            //if a candidate is still in the election - consider his vote as smallest
            min_vote = candidates[i].votes;
            //loop again over the participating candidates in order to update the min_vote if needed
            for (int j = 0; j < candidate_count; j++)
            {
                if (candidates[j].eliminated == false && candidates[j].votes < min_vote)
                {
                    min_vote = candidates[j].votes;
                }
            }
            return min_vote;
        }
    }
    return 0;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //loop over the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if a candidate is still in the election, but his num of votes doesnt equal to min - return false
        if (candidates[i].eliminated == false && candidates[i].votes != min)
        {
            return false;
        }
    }
    //otherwise - means everybody who's still there has the same vote as min - return true
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //loop over the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            //if a candidate has min number ov votes - change his eliminated status to true
            //change his votes to 0
            candidates[i].eliminated = true;
            candidates[i].votes = 0;
        }
    }
    return;
}