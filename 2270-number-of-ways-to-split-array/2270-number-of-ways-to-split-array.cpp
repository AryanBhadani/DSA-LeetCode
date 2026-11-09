class Solution {
public:
    int waysToSplitArray(vector<int>& nums) {
        int n=nums.size();
        long total=0,leftsum=0;
        int cnt=0;
        for(int i=0;i<n;i++){
            total+=nums[i];
        }
        for(int i=0;i<n-1;i++){
            leftsum+=nums[i];
            long rightsum=total-leftsum;
            if(leftsum>=rightsum){
                cnt++;
               
            }
        }
        return cnt;
        
    }
};