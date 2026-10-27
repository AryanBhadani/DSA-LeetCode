class Solution {
public:
int t[1001][1001];
    int longestPalindromeSubseq(string &s,string &rev, int m ,int n) {
        if(m==0||n==0){
            return 0;

        }
        if(t[m][n]!=-1){
            return t[m][n];
        }
        if(s[m-1]==rev[n-1]){
            return t[m][n]=1+longestPalindromeSubseq(s,rev,m-1,n-1);
        }
        else{
            return t[m][n]=max(longestPalindromeSubseq(s,rev,m-1,n),
                                longestPalindromeSubseq(s,rev,m,n-1));
        }
        
    }
    int longestPalindromeSubseq(string s){
        string rev=s;
        memset(t,-1,sizeof(t));
        reverse(rev.begin(),rev.end());
        int m =s.size();
        int n=rev.size();
        return longestPalindromeSubseq(s,rev,m,n); 
    }
};