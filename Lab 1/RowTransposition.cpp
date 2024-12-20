#include <iostream>
#include <string>
#include <vector>

using namespace std;

class RowTransposition{
    vector<vector<char>> matrix;
    vector<int> sequence;

    void createSequence(int l){
        //Create the random sequence of length l

        sequence.resize(l);

    }
public:
    RowTransposition(){
    }
    string encrypt(string text){
        createSequence(l);
        
        int l = floor(sqrt(text.length()));
        matrix.resize(l);
        for(vector<char> r: matrix){
            r.resize(l);
        }
        int index = 0;
        for(int r = 0;r < l;r++){
            for(int c = 0;c < l;c++){
                if(index < text.length()){
                    matrix[r][c] = text[index];
                    index++;
                }else{
                    matrix[r][c] = 'x';
                }
            }
        }
    }
    string decrypt(string text){
        int l = sqrt(text.length());
        matrix.resize(l);
        for(vector<char> r: matrix){
            r.resize(l);
        }
        int index = 0;
        for(int c = 0;c < l;c++){
            for(int r = 0;r < l;r++){
                matrix[r][c] = text[c*l + r];
            }
        }
    }
};

int main() {
    string plaintext;
    cout << "Enter plain text : ";
    cin >> plaintext;

    RowTransposition obj = RowTransposition();
    string encrypted = obj.encrypt(plaintext);
    string decrypted = obj.decrypt(encrypted);

    return 0;
}