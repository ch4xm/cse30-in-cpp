#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Words {
    vector<string> words;
    int count = 0;
};

struct SecretWord {
    string word;
    string guessed_letters;
    vector<char> chosen_letters;
};

string strip(const string& in){
    string out;

    for (char i : in){
        if (i != ' '){
            out += i;
        }
    }

    return out;
}

Words parse_words(const string& filepath){

    auto *words = new Words();

    ifstream infile(filepath);
    for (string line; getline(infile, line);) {
        words->words.push_back(strip(line));
        words->count++;
    }

    return *words;
}

SecretWord get_random_word(Words words) {
    auto *secret_word = new SecretWord;
    int ind = rand() % words.count;
    secret_word->word = words.words[ind];
    secret_word->guessed_letters = string(secret_word->word.length(), '_');
    return *secret_word;
}

bool make_guess(SecretWord &secret_word , char guess) {
    bool valid_guess = false;
    for (int i = 0; i < secret_word.word.length(); i++) {
        if (secret_word.word[i] == guess) {
            secret_word.guessed_letters[i] = guess;
            valid_guess = true;
        }
    }
    return valid_guess;
}

bool contains_letter(const vector<char>& v, char c) {
    for (char letter: v) {
        if (letter == c){
            return true;
        }
    }
    return false;
}

int main(){
    time_t t;
    srand((unsigned) time(&t));
    Words words = parse_words(R"(D:\Documents\Programming\C++\Hangman\dictionary.txt)");

    while (true) {

        int lives = 5;
        auto *guessed_letters = new vector<char>[26];


        cout << "Welcome to Hangman!" << endl;
        SecretWord word = get_random_word(words);

        while (true) {
            cout << word.guessed_letters << endl << "lives remaining: " << lives << endl;

            cout << "Which letter would you like to guess? ";
            char guess;
            cin >> guess;
            guess = tolower(guess); // Check if guess is valid alphabetical char
            if (!isalpha(guess)) {
                cout << "That is not a letter! Try again!" << endl;
                continue;
            }
            else if (contains_letter(word.chosen_letters, guess)) {
                cout << "You have already guessed the letter "<< guess << endl;
                continue;
            }

            if (make_guess(word, guess)){
                cout << "You guessed right!" << endl;
            }
            else {
                cout << "You guessed wrong and lost a life!" << endl;
                lives--;
            }
            word.chosen_letters.push_back(guess);

            if (word.word == word.guessed_letters) {
                cout << "You won! The word was " << word.word << endl;
                break;
            }
            else if (lives == 0){
                cout << "You lost! The word was " << word.word << "!" << endl;
                break;
            }
        }

        char choice;
        cout << "Play again (y/n): ";
        cin >> choice;
        if (tolower(choice) != 'y'){
            cout << "See you later! Quitting game..." << endl;
            break;
        }
    }

    return 0;
}