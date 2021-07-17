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
    ifstream input_file("C:/Users/Dell/Desktop/DD2_Final_pro/DD_project_Lee/test cases/test3.txt");
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

    /*
    for (int i = 0; i < nets.size(); i++) {
        for (int j = 0; j < nets[i].size(); j++) {
            cout << "Net Name: " << nets[i][j].net_name << " X: " << nets[i][j].x << " y: " << nets[i][j].y << " Metal: " << nets[i][j].M << endl;
        }
    }

    cout << "Obstacles: " << endl;
    for (int i = 0; i < obstacles.size(); i++) {
        cout << "X: " << obstacles[i].x << " y: " << obstacles[i].y << endl;
    }
    */
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

vector <vector <int>> lee(obj source, obj target, int M, int initial)
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
    /*
     cout << source.net_name << " : ";
     for (int j = path.size() - 1; j >= 0; j--)
            cout << "(" << path[j].M << " , " << path[j].x << " , " << path[j].y << ")" << "  ";
    */
     return path;

}

vector <bool> right_wrong(vector<obj> path_coordinates, int M) {

    vector <bool> check(path_coordinates.size(), false); //initialize eno kolo wrong

    check[0] = true;

    int j = path_coordinates.size() - 1;

    for (int i = 1; i < path_coordinates.size(); i++) {
        if (M == 1) {

            if (path_coordinates[j].x == path_coordinates[j - 1].x) {
                check[i] = true;
            }
        }
        else if (M == 2) {

            if (path_coordinates[j].y == path_coordinates[j - 1].y) {
                check[i] = true;
            }
        }

        j--;

    }

    //Printing boolean array
    /*
    cout << endl;
    for (int i = 0; i < check.size(); i++)
        cout << check[i] << " ";
    */
    return check;
}

void comparing(vector<bool> bool_arr, vector<obj>& efficient_path) {

    int wrong_num = 0;
    int p1 = 1, p2 = 0;
    int via_num = 0;

    bool flag = false; //eno hit true

    while (p2 != bool_arr.size() - 1 && p1 != bool_arr.size() - 1) {
        if (bool_arr[p1] == true) {
            p1++;
            flag = false;
        }
        else if (bool_arr[p1] == false) {
            p2 = p1;
            flag = true;
            while (bool_arr[p2 + 1] != true && p2 != bool_arr.size() - 2) { 
                p2++;
            }
            if (p2 == bool_arr.size() - 2) 
            {
                if (bool_arr[p2 + 1] != true)
                    p2++;
            }
        }

        if (flag) {
            wrong_num = p2 - p1 + 1;

            if (2 * via_cost + wrong_num < wrong_cost * wrong_num) {

                //cout << endl;
                //cout << "path size before insertion: " << efficient_path.size() << endl;
                //cout << p1 << endl;
                //cout << p2 << endl;

                for (int i = 0; i < wrong_num; i++) {
                    if (efficient_path[efficient_path.size() - 1 - (p1 + via_num + i)].M == 1) efficient_path[efficient_path.size() - 1 - (p1 + via_num + i)].M = 2; // Edited
                    else if (efficient_path[efficient_path.size() - 1 - (p1 + via_num + i)].M == 2) efficient_path[efficient_path.size() - 1 - (p1 + via_num + i)].M = 1; // Edited
                }

                //edit el nodes
                obj via1 = efficient_path[efficient_path.size() - 1 - (p1 + via_num - 1)]; 
                if (via1.M == 1) via1.M = 2; 
                else if (via1.M == 2) via1.M = 1;
                obj via2 = efficient_path[efficient_path.size() - 1 - (p2 + via_num)]; 
                if (via2.M == 1) via2.M = 2;
                else if (via2.M == 2) via2.M = 1;
                //cout << "(" << via1.M << "," << via1.x << "," << via1.y << ")" << endl;
                //cout << "(" << via2.M << "," << via2.x << "," << via2.y << ")" << endl;

                vector<obj>::iterator it;
                it = efficient_path.end() - (p1 + via_num); 
                it = efficient_path.insert(it, via1);
                it = efficient_path.end() - (p2 + via_num + 2); 
                it = efficient_path.insert(it, via2);

                via_num = via_num + 2;

                //cout << "path size now: " << efficient_path.size() << endl;

            }
            p1 = p2 + 1;
            flag = false;
            wrong_num = 0;
        }
    }
}

void test1(obj src, obj tar) {
    vector <obj> path = shortest_path(src, tar, lee(src,tar, src.M, 0));
    vector <bool> boolean = right_wrong(path, src.M);
    comparing(boolean, path);
    cout << path[0].net_name << " : ";
    for (int j = path.size() - 1; j >= 0; j--)
        cout << "(" << path[j].M << " , " << path[j].x << " , " << path[j].y << ")" << "  ";

}

void test2(obj src, obj tar) {

    tar.M = src.M;
    obj via;
    vector <obj> path = shortest_path(src, tar, lee(src, tar, src.M, 0));
    vector <bool> boolean = right_wrong(path, src.M);
    comparing(boolean, path);
    vector<obj>::iterator it;

    if (boolean[boolean.size() - 1] == true) {

        via = tar;
        if (tar.M == 1) path[0].M = 2;
        else if (tar.M == 2) path[0].M = 1;

        it = path.begin() +1;
        it = path.insert(it, via);
    }
    if (boolean[boolean.size() - 1] == false) {
    
        if (path[0].x == path[1].x && path[0].y == path[1].y) {

            path.erase(path.begin());
        }
        else {
            //via 3nd correct
            //8aiar el M bta3et kol elly b3dha
            int indx = 0;
            for (int i = boolean.size() - 1; i >= 0; i--) {
                if (boolean[i] == false) {
                    indx++;
                }
                else
                {
                    break;
                }
            }

            via = path[indx];
            if (via.M == 1) via.M = 2;
            else if (via.M == 2) via.M = 1;
            for (int i = 0; i < indx; i++) {
                if (path[i].M == 1) path[i].M = 2;
                else if (path[i].M == 2) path[i].M = 1;
            }

            it = path.begin() + indx;
            it = path.insert(it, via);

        }

    }

    cout << path[0].net_name << " : ";
    for (int j = path.size() - 1; j >= 0; j--)
        cout << "(" << path[j].M << " , " << path[j].x << " , " << path[j].y << ")" << "  ";

}

int main() {
    init_file();
    resizing();
    test2(nets[0][0], nets[0][1]);

  
    return 0;
}