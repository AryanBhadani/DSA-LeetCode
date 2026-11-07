class Solution {
public:
    int countDigits(int num) {
        int cnt=0;
        int val=num;
        while(val>0){
            int dig=val%10;
            if(num%dig==0){
                cnt++;
            }
            val/=10;
        }
        return cnt;
    }
};