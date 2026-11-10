class Solution {
public:
    int percentageLetter(string s, char letter) {
        int S=s.size();
        int cnt=0;
        for(int i=0;i<S;i++){
            if(s[i]==letter){
                cnt++;
            }
        }
        return (cnt*100)/S;
        
    }
};