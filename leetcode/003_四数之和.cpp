#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int> > fourSum(vector<int>& nums, int target) {
        
        vector<vector<int> > myv;
        int n = nums.size();
        if(n < 4) return {};
        sort(nums.begin(), nums.end());
        int max3 = nums[n-3] + nums[n-2] + nums[n-1];
        int max2 = nums[n-2] + nums[n-1];

        for(int i = 0; i < n - 3; i++)
        {
            if(nums[i] * 4 > target) break;
            if(nums[i] + max3 < target) continue;
            if(i > 0 && nums[i-1]==nums[i]) continue;
            for(int j = i + 1; j < n - 2; j++)
            {
                if((nums[i]+nums[j])*2 > target) break;
                if(nums[i]+nums[j]+max2 < target) continue;
                if(j > i + 1 && nums[j-1]==nums[j]) continue;
                int l = j+1;
                int r = n-1;
                while(l<r)
                {
                    int sum = nums[i]+nums[j]+nums[l]+nums[r];
                    if(sum - target == 0)
                    {
                        vector<int> v;
                        v.push_back(nums[i]);
                        v.push_back(nums[j]);
                        v.push_back(nums[l]);
                        v.push_back(nums[r]);
                        myv.push_back(v);
                        while(l<r && nums[l]==nums[++l]);
                        while(l<r && nums[r]==nums[--r]);
                    }
                    else if(sum - target > 0)
                    {
                        r--;
                    }
                    else if(sum - target < 0)
                    {
                        l++;
                    }
                }
            }
        }
        return myv;
    }
};

void print(int n)
{
  cout << n << " ";
}

int main()
{
  Solution solution;
  vector<int> nums;
  nums.push_back(1);
  nums.push_back(0);
  nums.push_back(-1);
  nums.push_back(0);
  nums.push_back(-2);
  nums.push_back(2);

  vector<vector<int> > myv;
  myv = solution.fourSum(nums, 0);
  
  for(vector<vector<int> >::iterator start = myv.begin(); start != myv.end(); start++)
  {
    vector<int> item_v = *start;
    for(vector<int>::iterator s = item_v.begin(); s != item_v.end(); s++)
    {
      cout << *s << " ";
    }
    cout << endl;
  }

  return 0;
}
