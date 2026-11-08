class Solution {
public:
    vector<int> findWordsContaining(vector<string>& words, char x) {
        vector<int>ans;
        int w=words.size();
        for(int i=0;i<w;i++){
            if(count(words[i].begin(),words[i].end(),x)>0)
            ans.push_back(i);
        }
        return ans;
    }
};