class Solution {
public:
    int minOperations(vector<int>& nums) {
        int n =nums.size();
        int op=0;
        for(int i=1;i<n;i++){
            if(nums[i]<=nums[i-1]){
                op=op+nums[i-1]+1-nums[i];
                nums[i]=nums[i-1]+1;
            }
        }
        return op;
    }
};