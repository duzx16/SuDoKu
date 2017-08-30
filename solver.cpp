#include <iostream>
#include  "solver.h"

using namespace std;

Sudoku_solver::Sudoku_solver(int (*init)[9])
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
            data[i][j] = init[i][j];
    }
}

bool Sudoku_solver::check_answer(int row, int column, int answer)
{
    for (int i = 0; i < 9; ++i)
    {
        if (i != row && data[i][column] == answer)
            return false;
        if (i != column && data[row][i] == answer)
            return false;
    }
    int l_row = row / 3, l_column = column / 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            if ((i != row % 3 || j != column % 3) && data[l_row * 3 + i][l_column * 3 + j] == answer)
                return false;
        }

    return true;
}

bool Sudoku_solver::trace_back(int i, int j)
{
    if (i == 8 && j == 9)
    {
        solutions.push_back(data);
        return true;
    }
    if (j == 9)
    {
        i += 1;
        j = j % 9;
    }
    if (data[i][j] == 0)
    {
        for (int answer = 1; answer <= 9; ++answer)
        {
            if (check_answer(i, j, answer))
            {
                data[i][j] = answer;
                trace_back(i, j + 1);
                data[i][j] = 0;
            }
        }
    } else
    {
        return trace_back(i, j + 1);
    }
    return false;
}

bool Sudoku_solver::solve()
{
    return trace_back(0,0);
}

void Sudoku_solver::print_sudoku(Sudoku_array s)
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            cout<<s[i][j]<<'\t';
        }
        cout<<'\n';
    }
}

void Sudoku_solver::print_result()
{
    if(solutions.empty())
        cout<<"No solutions\n";
    else if(solutions.size()==1)
    {
        cout<<"There is only one solution\n";
        print_sudoku(solutions[0]);
    }
    else
    {
        cout<<"There are more than one solution\n";
    }
}

Sudoku_solver::Sudoku_solver(Sudoku_array aim):data(aim)
{

}

Sudoku_solver::Sudoku_solver()
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            data[i][j]=0;
        }
    }
}

Solvable_judge::Solvable_judge()
{

}

bool Solvable_judge::trace_back(int i, int j)
{
    if (i == 8 && j == 9)
    {
        solutions.push_back(data);
        return true;
    }
    if (j == 9)
    {
        i += 1;
        j = j % 9;
    }
    if (data[i][j] == 0)
    {
        for (int answer = 1; answer <= 9; ++answer)
        {
            if (check_answer(i, j, answer))
            {
                data[i][j] = answer;
                if(trace_back(i, j + 1))
                    return true;
            }
        }
        data[i][j] = 0;
    } else
    {
        return trace_back(i, j + 1);
    }
    return false;
}

void Generator::generate_pattern()
{
    do
    {
        init_data();
        init_pattern(11);
    }
    while(!solve());
}

bool Generator::init_pattern(int n)
{
    std::uniform_int_distribution<int> rand_choice(1,9);
    for(int num=0;num<n;++num)
    {
        int i,j,answer;
        do
        {
            i=rand_choice(eng)-1;
            j=rand_choice(eng)-1;
        }
        while(data[i][j]!=0);
        do
        {
            answer=rand_choice(eng);
        }
        while(!check_answer(i,j,answer));
        data[i][j]=answer;
    }
    return true;
}

Generator::Generator():Solvable_judge(),eng(time(NULL))
{
    for(int i=0;i<10;++i)
        for(int j=0;j<2;++j)
            complexity_block[i][j]=i*3+j+27;
}

void Generator::init_data()
{
    for(int i=0;i<9;++i)
        for(int j=0;j<9;++j)
        {
            data[i][j]=0;
        }
}

bool Generator::dig_hole(int min, int max)
{
    array<int,81> dig_step;
    for(int i=0;i<9;++i)
        for(int j=0;j<9;++j)
        {
            dig_step[i*9+j]=i*9+j;
        }
    std::uniform_int_distribution<int> final_choose(min,max);
    int final_digs=final_choose(eng);
    shuffle(dig_step.begin(),dig_step.end(),eng);
    int cur=0;
    for(int i=0;i<final_digs;++i)
    {
        do
        {
            if(cur==81)
            {
                return i>=min-1&&i<=max-1;
            }
            int row=dig_step[cur]/9,column=dig_step[cur]%9,value=data[row][column];
            data[row][column]=0;
            Sudoku_solver s(data);
            s.solve();
            cur++;
            if(s.one_sol())
            {
                data[row][column]=0;
                break;
            }
            data[row][column]=value;
        }
        while(true);
    }
    return true;
}

void Generator::generate(int complexity)
{
    generate_pattern();
    int min=complexity_block[complexity][0],max=complexity_block[complexity][1];
    //cout<<min<<','<<max<<'\n';
    while(!dig_hole(min,max))
    {
        std::cout<<"Fail\n";
        generate_pattern();
        std::cout<<"Restart\n";
    }
    shuffle_data();
    //print_sudoku(data);
    //Sudoku_solver s(data);
    //s.solve();
    //s.print_result();
}

Sudoku_array Generator::result()
{
    return data;
}

void Generator::swap_digit(int n1, int n2)
{
    for(int i=0;i<9;++i)
        for(int j=0;j<9;++j)
        {
            if(data[i][j]==n1)
                data[i][j]=n2;
            else if(data[i][j]==n2)
                data[i][j]=n1;
        }
}

void Generator::swap_column(int c1, int c2)
{
    for(int i=0;i<9;++i)
    {
        std::swap(data[i][c1],data[i][c2]);
    }
}

void Generator::swap_row(int r1, int r2)
{
    for(int i=0;i<9;++i)
    {
        std::swap(data[r1][i],data[r2][i]);
    }
}

void Generator::shuffle_data()
{
    std::uniform_int_distribution<int> rand(0,2);
    std::uniform_int_distribution<int> rand_choice(1,9);
    int r1=30,r2=10;
    for(int i=0;i<r1;++i)
    {
        int d1=rand_choice(eng),d2=rand_choice(eng);
        swap_digit(d1,d2);
    }
    for(int i=0;i<r2;++i)
    {
        int d1=rand(eng),d2=rand(eng),d3=rand(eng);
        swap_column(d1*3+d2,d1*3+d3);
    }
    for(int i=0;i<r2;++i)
    {
        int d1=rand(eng),d2=rand(eng),d3=rand(eng);
        swap_row(d1*3+d2,d1*3+d3);
    }
}
