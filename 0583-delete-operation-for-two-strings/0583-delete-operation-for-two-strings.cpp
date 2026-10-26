class Solution {
public:
int t[501][501];
    int minDistance(string &word1, string &word2,int m, int n) {
        if(m==0||n==0){
            return 0;
        }
        if(t[m][n]!=-1){
            return t[m][n];
        }
        if(word1[m-1]==word2[n-1]){
            return t[m][n]=1+minDistance(word1,word2,m-1,n-1);
        }
        else{
            return t[m][n]=max(minDistance(word1,word2,m-1,n),
                           minDistance(word1,word2,m,n-1));
        }
        
    }
    int minDistance(string &word1,string &word2){
        memset(t,-1,sizeof(t));
        int m=word1.size();
        int n =word2.size();
        return m+n-2*minDistance(word1,word2,m,n);
    }
};