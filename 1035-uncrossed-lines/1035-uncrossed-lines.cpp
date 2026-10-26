class Solution {
public:
int t[501][501];
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2,int m ,int n) {
        if(m==0||n==0){
            return 0;
        }
        if(t[m][n]!=-1){
            return t[m][n];
        }
        if(nums1[m-1]==nums2[n-1]){
            return t[m][n]=1+maxUncrossedLines(nums1,nums2,m-1,n-1);
        }
        else{
            return t[m][n]=max(maxUncrossedLines(nums1,nums2,m-1,n),
                           maxUncrossedLines(nums1,nums2,m,n-1));
        }
        
    }
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        memset(t,-1,sizeof(t));
        return maxUncrossedLines(nums1, nums2, nums1.size(), nums2.size());
    }
};