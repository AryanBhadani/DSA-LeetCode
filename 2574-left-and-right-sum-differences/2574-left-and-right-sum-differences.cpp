class Solution {
public:
    vector<int> leftRightDifference(vector<int>& nums) {
        int leftsum=0;
        int rightsum=0;
        int n=nums.size();
        vector<int> ans;
        for(int i=0;i<n;i++){
            rightsum +=nums[i];
        }
        for(int i=0;i<n;i++){
            rightsum -=nums[i];
            ans.push_back(abs(leftsum-rightsum));
            leftsum +=nums[i];
        }
    return ans;    
    }
};