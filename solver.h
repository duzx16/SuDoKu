#ifndef SOLVER_H
#define SOLVER_H

#include <array>
#include <vector>
#include <random>

typedef std::array<std::array<int, 9>, 9> Sudoku_array;

class Sudoku_solver {
public:
    explicit Sudoku_solver(int (*init)[9]);
    Sudoku_solver(Sudoku_array aim);
    Sudoku_solver();
    bool solve();
    void print_result();
    bool one_sol(){return solutions.size()==1;}
protected:
    void init_mark();

    int block_num(int i,int j){return i/3*3+j/3;}

    bool check_answer(int row,int column,int answer);

    virtual bool trace_back(int i, int j);

    void print_sudoku(Sudoku_array s);

    Sudoku_array data;
    std::vector<Sudoku_array> solutions;

    bool row[9][9];
    bool column[9][9];
    bool block[9][9];

};

class Solvable_judge:public Sudoku_solver{
public:
    explicit Solvable_judge(int (*init)[9]):Sudoku_solver(init){}

    explicit Solvable_judge(Sudoku_array &aim):Sudoku_solver(aim){}

    Solvable_judge();
protected:

    bool trace_back(int i, int j);

};

class Generator : public Solvable_judge {
public:
    Generator();

    void generate(int complexity);

    Sudoku_array result();

    Sudoku_array return_answer(){return answer;}

private:
    bool init_pattern(int n);

    void init_data();

    void generate_pattern();

    bool dig_hole(int min, int max,int bound);

    bool check_lower_bound(int row,int col,int bound);

    void shuffle_data();

    void swap_digit(int n1, int n2);

    void swap_column(int c1, int c2);

    void swap_row(int r1, int r2);

    int complexity_block[10][2];

    int lower_bound[10];

    std::mt19937 eng;

    Sudoku_array answer;

};

#endif // SOLVER_H
