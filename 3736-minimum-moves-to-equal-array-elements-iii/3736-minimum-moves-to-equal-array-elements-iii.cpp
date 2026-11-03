class Solution {
public:
    int minMoves(vector<int>& nums) {
        int n=nums.size();
        int max=0;
        for(int i=0;i<n;i++){
            if(nums[i]>max){
                max=nums[i];
            }
        }
        int move=0;
        for(int i=0;i<n;i++){
            move=move+max-nums[i];
        }
        return move;
    }
};