class Solution {
public:
    int deleteAndEarn(vector<int>& nums,vector<int>& dp,int i){
        if(i>=nums.size()){
            return 0;
        }
        if(dp[i]!=-1){
            return dp[i];
        }
        int take=nums[i]+deleteAndEarn(nums,dp,i+2);
        int skip=deleteAndEarn(nums,dp,i+1);
        dp[i]=max(take,skip);
        return dp[i];
    }
    int deleteAndEarn(vector<int>& nums){
        if(nums.empty()){
            return 0;
        }
        int maxi=*max_element(nums.begin(),nums.end());
        vector<int> points(maxi+1,0);
        for(int i=0;i<nums.size();i++){
            points[nums[i]]+=nums[i];
        }
        vector<int> dp(points.size(),-1);
        return deleteAndEarn(points,dp,0);
    }
};