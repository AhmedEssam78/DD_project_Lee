#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <iomanip>
using namespace std;


//struct for obstacles & nets 
struct obj {
    int x, y;
    int M;
    string net_name;
    obj() {
        x = 0;
        y = 0;
        M = 0;
        net_name = "";
    }

    obj(int xx, int yy, int MM, string net_namee)
    {
        x = xx;
        y = yy;
        M = MM;
        net_name = net_namee;
    }
};

vector <obj> obstacles(1);
vector <vector<obj>> nets(1);
vector <vector <int>> H_grid(1);
vector <vector <int>> V_grid(1);
int x_grid, y_grid;
int via_cost;
int wrong_cost;


void init_file() {
    ifstream input_file("C:/Users/user/Downloads/DD2_Project/DD2/input.txt");
    string line;

    if (!input_file.is_open())
    {
        cout << "error opening the input_file!!" << endl;
        return;
    }
    else
    {
        getline(input_file, line);
        stringstream data(line);
        string str;
        getline(data, str, 'x');
        x_grid = stoi(str);
        getline(data, str, '\n');
        y_grid = stoi(str);

        int i = 0, j = 0;

        while (getline(input_file, line)) {
            if (line[0] == 'O') {
                obstacles.resize(i + 1);
                line.erase(0, 5);
                stringstream data(line);
                getline(data, str, ',');
                obstacles[i].x = stoi(str);
                getline(data, str, ')');
                str.erase(0, 1);
                obstacles[i].y = stoi(str);
                i++;
            }
            else {
                int counter = 0;
                nets.resize(j + 1);
                for (int i = 0; i < line.length(); i++)
                    if (line[i] == '(') counter++;
                nets[j].resize(counter);
                stringstream data(line);
                getline(data, str, ' ');

                //name
                for (int i = 0; i < counter; i++)
                    nets[j][i].net_name = str;

                for (int i = 0; i < counter; i++) {

                    getline(data, str, ',');
                    str.erase(0, 1);
                    //cout << str << endl;
                    nets[j][i].M = stoi(str);
                    getline(data, str, ',');
                    str.erase(0, 1);
                    //cout << str << endl;
                    nets[j][i].x = stoi(str);
                    getline(data, str, ')');
                    str.erase(0, 1);
                    //cout << str << endl;
                    nets[j][i].y = stoi(str);
                    getline(data, str, ' ');
                }

                j++;
            }

        }
    }

    input_file.close();
    cout << "Please, Enter the cost of the Via: ";
    cin >> via_cost;
    cout << endl << "Please, Enter the cost of the Wrong Path: ";
    cin >> wrong_cost;
    for (int i = 0; i < nets.size(); i++) {
        for (int j = 0; j < nets[i].size(); j++) {
            cout << "Net Name: " << nets[i][j].net_name << " X: " << nets[i][j].x << " y: " << nets[i][j].y << " Metal: " << nets[i][j].M << endl;
        }
    }

    cout << "Obstacles: " << endl;
    for (int i = 0; i < obstacles.size(); i++) {
        cout << "X: " << obstacles[i].x << " y: " << obstacles[i].y << endl;
    }

}

void resizing() {
    H_grid.resize(x_grid);
    V_grid.resize(x_grid);
    for (int i = 0; i < x_grid; i++)
    {
        H_grid[i].resize(y_grid);
        V_grid[i].resize(y_grid);
    } 
}

vector <vector <int>> numbering(obj source, obj target, int M, int initial)
{
    vector <vector <int>> matrix(x_grid);
    vector <vector <int>> visited(x_grid);
    for (int i = 0; i < x_grid; i++)
    {
        visited[i].resize(y_grid);
        matrix[i].resize(y_grid);
        for (int j = 0; j < y_grid; j++)
        {
            visited[i][j] = 0;
            matrix[i][j] = INT_MAX;
        }
    }


    for (int i = 0; i < obstacles.size(); i++)
        visited[obstacles[i].x][obstacles[i].y] = -2;

     matrix[source.x][source.y] = initial;
     matrix[target.x][target.y] = 100000;
        

     //Applying lee's algorithm
     int move_x[] = { -1, 0, 1, 0 };
     int move_y[] = { 0, 1, 0, -1 };

     queue<int> X, Y;

     X.push(source.x);
     Y.push(source.y);
     int current_x, current_y, xx, yy;
     bool reached = false;
     while (!X.empty() && !reached)
     {
         current_x = X.front();
         current_y = Y.front();

         visited[current_x][current_y] = -1;
         for (int i = 0; i < 4; i++)
         {
             xx = current_x + move_x[i];
             yy = current_y + move_y[i];

             if (xx >= 0 && yy >= 0 && xx < x_grid && yy < y_grid)
             {
                 if (matrix[xx][yy] == 100000)
                     reached = true;
                 if (visited[xx][yy] != -1 && visited[xx][yy] != -2 && matrix[xx][yy] != 100000)
                 {
                     X.push(xx);
                     Y.push(yy);
                     visited[xx][yy] = -1;
                     if (M == 1) // Horizontal (xx - current_x = 0)
                     {
                         if (xx == current_x)
                            matrix[xx][yy] = matrix[current_x][current_y] + 1;
                         else 
                            matrix[xx][yy] = matrix[current_x][current_y] + wrong_cost;
                     }
                     else if (M == 2) //Vertical (yy - current_y = 0)
                     {
                         if (yy == current_y)
                             matrix[xx][yy] = matrix[current_x][current_y] + 1;
                         else
                             matrix[xx][yy] = matrix[current_x][current_y] + wrong_cost;
                     }
                 }
             }
         }

         X.pop();
         Y.pop();
     }

     // Printing visited 
     for (int i = 0; i < x_grid; i++)
     {
         for (int j = 0; j < y_grid; j++)
         {
             if (visited[i][j] == -2)
                 cout << setw(10) << "O";
             else
                 cout << setw(10) << visited[i][j];
         }
         cout << endl;
     }

     cout << endl;

     // Printing matrix
     for (int i = 0; i < x_grid; i++)
     {
         for (int j = 0; j < y_grid; j++)
         {
             if (matrix[i][j] == INT_MAX)
                 cout << setw(10) << "O";
             else
                 cout << setw(10) << matrix[i][j];
         }
         cout << endl;
     }

     cout << endl;

     //Coppying the matrix into the suitable grid
     if (M == 1)
     {
         for (int i = 0; i < x_grid; i++)
             for (int j = 0; j < y_grid; j++)
                 H_grid[i][j] = matrix[i][j];
         return H_grid;
     }
     else if (M == 2)
     {
         for (int i = 0; i < x_grid; i++)
             for (int j = 0; j < y_grid; j++)
                 V_grid[i][j] = matrix[i][j];
         return V_grid;
     }
}

vector <obj> shortest_path(obj source, obj target, vector <vector <int>> matrix) {
    //Finding the shortest path
    vector <obj> path;
    int x = target.x;
    int y = target.y;
    obj tar(x, y, target.M, target.net_name); 
    path.push_back(tar);
    int move_x[] = { -1, 0, 1, 0 };
    int move_y[] = { 0, 1, 0, -1 };
    while (matrix[x][y] != 0)
    {
        int xxx, yyy;
        int min = INT_MAX;
        int x_path, y_path;
        for (int k = 0; k < 4; k++)
        {
            xxx = x + move_x[k];
            yyy = y + move_y[k];
            if (xxx >= 0 && xxx < x_grid && yyy >= 0 && yyy < y_grid) {
                if (matrix[xxx][yyy] < min)
                {
                    min = matrix[xxx][yyy];
                    x_path = xxx; y_path = yyy;
                }
            }
        }

        obj ob(x_path, y_path, source.M, source.net_name);  ////////////// In case : same metal layer
        path.push_back(ob);
        x = x_path; y = y_path;
    }


    // Printing path
     cout << source.net_name << " : ";
     for (int j = path.size() - 1; j >= 0; j--)
            cout << "(" << path[j].x << " , " << path[j].y << ")" << "  ";

     return path;

}

int main() {
    init_file();
    resizing();
    vector <obj> path = shortest_path(nets[0][0], nets[0][1], numbering(nets[0][0], nets[0][1], nets[0][0].M, 0));
    return 0;
}