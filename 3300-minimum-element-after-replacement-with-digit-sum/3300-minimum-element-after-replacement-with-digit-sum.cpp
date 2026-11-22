class Solution {
public:
    int minElement(vector<int>& nums) {
        int ans=INT_MAX;
        int n=nums.size();
        for(int i=0;i<n;i++){
            int digitsum=0;
            while(nums[i]>0){
                digitsum+=nums[i]%10;
                nums[i]/=10;
            }
            ans=min(ans,digitsum);

        }
        return ans;
        
    }
};