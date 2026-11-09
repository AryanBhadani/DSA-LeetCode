class Solution {
public:
    int findMiddleIndex(vector<int>& nums) {
        int total =0, leftSum =0;
        int n=nums.size();
        for(int i=0;i<n;i++){
            total +=nums[i];
            }


        for(int i =0;i<n; i++) {
            int rightSum=total-leftSum -nums[i];

            if(leftSum==rightSum)
                return i;

            leftSum += nums[i];
        }

        return -1;
    }
};