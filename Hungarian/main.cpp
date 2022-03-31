#include<iostream>
#include<stdlib.h>
#include<vector>
#include <algorithm>
using namespace std;

#define RANGE 1000

void solve(int (&Cost)[3][4], const int N, const int M, const int MODE, int *assignment_index){
  std::cout<<"********Step1*************\n";
  //step 1: minimum element in each row is subtracted from all the elements in that row
  for(int i=0; i<N; i++){
    int min_cost = RANGE;
    for (int j = 0; j < M; j++) {
      if (Cost[i][j] < min_cost){
          min_cost = Cost[i][j];
      }
    }
    for (int j=0; j<M; j++){
      Cost[i][j] = Cost[i][j] - min_cost;
    }

  }

  for( int i = 0; i < N; i++){
    for( int j = 0; j < M; j++) {
      std::cout<<Cost[i][j]<<'\t';
    }
    std::cout<<'\n';
  }

  //check if assignment is possibles
  std::vector<int> crossed_cols_1;
  for(int i=0; i<N; i++){
    for (int j = 0; j < M; j++){
      if (!(std::count(crossed_cols_1.begin(), crossed_cols_1.end(), j))){
        if (Cost[i][j] == 0){
            assignment_index[i] = j;
            crossed_cols_1.push_back(j);
            break;
        }
      }
    }
  }
  std::cout << '\n';
  //if all the tasks can be assigned then finish, if not keep going:
  bool done = true;
  for( int i = 0; i < N; ++i){
      if (assignment_index[i] == -1){
        done = false;
      }
  }

  if (done) return;

  std::cout<<"********Step2*************\n";
  //step2: minimum element in each column is subtracted from all the elements in that column
  for(int i=0; i<M; i++){
    int min_cost = RANGE;
    for (int j = 0; j < N; j++) {
      if (Cost[j][i] < min_cost){
          min_cost = Cost[j][i];
      }
    }
    for (int j=0; j<N; j++){
      Cost[j][i] = Cost[j][i] - min_cost;
    }
  }

  for( int i = 0; i < N; ++i){
    for( int j = 0; j < M; ++j) {
      std::cout<<Cost[i][j]<<'\t';
    }
    std::cout<<'\n';
  }
  ////assign as many tasks as possible.
  std::vector<int> crossed_cols;
  for(int i=0; i<N; i++){
    for (int j = 0; j < M; j++){
      if (!(std::count(crossed_cols.begin(), crossed_cols.end(), j))){
        if (Cost[i][j] == 0){
            assignment_index[i] = j;
            crossed_cols.push_back(j);
            break;
        }
      }
    }
  }
  //if all the tasks can be assigned then finish, if not keep going:
  done = true;
  for( int i = 0; i < N; i++){
      if (assignment_index[i] == -1){
        done = false;
      }
  }
  if (done) return;

  std::cout << '\n';
  while(true) {
    std::cout<<"********Step3*************\n";
    //step3:All zeros in the matrix must be covered by marking as few rows and/or columns as possible.
    ///Marking
    ////Mark all rows having no assignments
    std::vector<int> marked_rows;
    for( int i = 0; i < N; ++i){
        if (assignment_index[i] == -1 ){
          marked_rows.push_back(i);
        }
    }
    std::vector<int> marked_cols;
    std::vector<int> new_marked_rows;
    new_marked_rows = marked_rows;
    std::vector<int> new_marked_cols;

    int mark_count = 0;
    while (true) {
      int new_mark_col_count = 0;
      int new_mark_rows_count = 0;
      ////Mark all columns having zeros in newly marked row
      for(int i=0; i<M; i++){
        if (std::count(new_marked_rows.begin(), new_marked_rows.end(), i)){
          for (int j = 0; j < N; j++) {
            if ((Cost[i][j] == 0) && (!std::count(new_marked_cols.begin(), new_marked_cols.end(), j))){
              new_marked_cols.push_back(j);
              new_mark_col_count ++;
              marked_cols.push_back(j);
            }
          }
        }
      }

      ///Mark all rows having assignments in newly marked columns
      for (int j = 0; j < M; j++) {
        if (std::count(new_marked_cols.begin(), new_marked_cols.end(), j)){
          for (size_t i = 0; i < N; i++) {
            if (Cost[i][j] == 0){
              for (int k = 0; k < N; k++) {
                if ((assignment_index[k] == i) && (!std::count(new_marked_rows.begin(), new_marked_rows.end(), i))){
                  new_marked_rows.push_back(i);
                  new_mark_rows_count ++;
                  marked_rows.push_back(i);
                }
              }
            }
          }
        }
      }
      if ((new_mark_col_count == 0) && (new_mark_rows_count == 0)){
        break;
      }

      if (mark_count > 0) {
        //keep only newly marked cols
        new_marked_cols.erase(std::next(new_marked_cols.begin(), 0), std::next(new_marked_cols.begin(), new_marked_cols.size() - new_mark_col_count));
      }
      //keep only newly marked rows
      new_marked_rows.erase(std::next(new_marked_rows.begin(), 0), std::next(new_marked_rows.begin(), new_marked_rows.size() - new_mark_rows_count));

      mark_count++;
    }
    std::cout<<"********marked rows********\n";
    for( int i = 0; i < marked_rows.size(); ++i){
        std::cout<<marked_rows[i]<<'\t';
    }
    std::cout<<"\n********marked cols********\n";

    for( int i = 0; i < marked_cols.size(); ++i){
        std::cout<<marked_cols[i]<<'\t';
    }
    std::cout << "lines: " << (marked_rows.size() + marked_cols.size()) << '\n';
    if ((marked_rows.size() + marked_cols.size()) == std::min(N,M)){
      break;
    }
    std::cout<<"\n********Step4********\n";
    //minimum of unmarked elements:
    int min_el_left = 10000;
    std::vector<float> els_left;
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
        if ((std::count(marked_rows.begin(), marked_rows.end(), i)) && (!std::count(marked_cols.begin(), marked_cols.end(), j))){
          std::cout << "index: " << i << "," << j << '\n';
          if (Cost[i][j] < min_el_left){
            min_el_left = Cost[i][j];
          }
        }
      }
    }

    // Subtract the min from every unmarked element
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
        if ((std::count(marked_rows.begin(), marked_rows.end(), i)) && (!std::count(marked_cols.begin(), marked_cols.end(), j))){
            Cost[i][j] = Cost[i][j] - min_el_left;
        }
      }
    }
    //add min to every element covered by two lines
    for (int i = 0; i < marked_rows.size(); i++) {
      for (int j = 0; j < marked_cols.size(); j++) {
        Cost[marked_rows[i]][marked_cols[j]] = Cost[marked_rows[i]][marked_cols[j]] + min_el_left;
      }
    }

    for( int i = 0; i < N; ++i){
      for( int j = 0; j < M; ++j) {
        std::cout<<Cost[i][j]<<'\t';
      }
      std::cout<<'\n';
    }
  }
}

int main() {

 srand(time(0));
 int N = 3;
 int M = 4;
 //float **Cost;
 //Cost = new float *[N];
 //for(int i = 0; i <N; i++)
 //   Cost[i] = new float[M];

 int *assignment_index;
 assignment_index = new int[N];
 //initialize with -1
 for(int i = 0; i <N; i++)
    assignment_index[i] = -1;

 int Cost[3][4] = {
        3, 7, 5, 11,
        5, 4, 6, 3,
        4, 10, 1, 2,
        };

 for( int i = 0; i < N; i++){
   for( int j = 0; j < M; j++) {
     std::cout<<Cost[i][j]<<'\t';
   }
   std::cout<<'\n';
 }
 const int MODE = 0;
 solve(Cost, N, M, MODE, assignment_index);

 std::cout<<"********Assignment********\n";
 for( int i = 0; i < N; ++i){
     std::cout<<assignment_index[i]<<'\t';
 }
 std::cout<<'\n';

 return 0;
}