class Solution {
public:
    string convertToTitle(int columnNumber) {
        string ans = "";
        int ct =columnNumber;

        while(ct>0) {
            ct--;

            int rem=ct%26;

            char ch='A'+rem;
            ans =ch+ans;

            ct=ct/26;
        }

        return ans;
    }
};