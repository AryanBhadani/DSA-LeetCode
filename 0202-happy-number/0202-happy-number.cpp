class Solution {
public:
    bool isHappy(int n) {
        while (true){
            if (n==1)
                return true;
            else if(n==4)   //Wapas 4! Cycle repeat hota rahega isliye false hogi number
                return false;

            int sum=0;

            while(n>0){
                int rem=n%10;
                sum=sum+rem*rem;
                n=n/10;
            }

            n=sum;
        }
    }
};