void find_Playfair(string plaintext, string key)
{
    int N=5;
    int key_len=key.length();
    int put_count=0;
    char mat[5][5];
    int k='A';
    cout<<key_len<<endl;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(put_count<key_len){
                mat[i][j] = toupper(key[put_count++]);
            }
            else {
                bool found = false;
                while (!found) {
                    found = true;
                    for (int x = 0; x < key_len; x++) {
                        if (toupper(key[x]) == (char)k || (char)k == 'J') {
                            found = false;
                            k++;
                            break;
                        }
                    }
                }
                mat[i][j] = (char)k;
                k++;
            }
        }
    }
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
}
