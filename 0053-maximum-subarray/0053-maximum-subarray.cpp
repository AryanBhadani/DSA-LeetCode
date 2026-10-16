class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int currentsum = nums[0];
        int largestsum = nums[0];
        int n =nums.size();
        for(int i=1;i<n;i++){
            if (currentsum + nums[i] > nums[i]) {
                currentsum = currentsum + nums[i];
            } 
            else {
                currentsum = nums[i];
            }
            if (currentsum>largestsum) {
                largestsum = currentsum;
            }
        }
        return largestsum;
        
    }
};