
class Solution {
public:
    int t[1001][1001];

    int minimumDeleteSum(string &s1,string &s2, int m,int n) {
        if (m==0) {
            int sum = 0;
            for (int i=0;i<n;i++) {
                sum=sum+s2[i];
            }
            return sum;
        }

        if (n==0) {
            int sum=0;
            for (int i=0;i<m;i++) {
                sum=sum+s1[i];
            }
            return sum;
        }

        if (t[m][n]!=-1) {
            return t[m][n];
        }

        if (s1[m-1]==s2[n-1]) {
            return t[m][n] =
                minimumDeleteSum(s1,s2,m-1,n-1);
        }
        else {
            return t[m][n] = min(
                (int)s1[m-1]+minimumDeleteSum(s1,s2,m-1,n),
                (int)s2[n-1]+minimumDeleteSum(s1,s2,m,n-1)
            );
        }
    }

    int minimumDeleteSum(string s1, string s2) {
        memset(t,-1,sizeof(t));
        int m = s1.size();
        int n = s2.size();
        return minimumDeleteSum(s1,s2,m,n);
    }
};
