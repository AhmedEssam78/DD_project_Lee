#include <iostream> 
#include <cmath>
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
        M = 1;
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
vector <obj> obs_visual;
vector <vector<obj>> nets(1);
vector <vector <obj>> final_path;
vector <vector <int>> H_grid(1);
vector <vector <int>> V_grid(1);
int x_grid, y_grid;
int via_cost;
int wrong_cost;


int init_file() {
    ifstream input_file("C:/Users/Dell/Desktop/DD2_Final_pro/DD_project_Lee/test cases/test8.txt");
    string line;

    if (!input_file.is_open())
    {
        cout << "error opening the input_file!!" << endl;
        return 0;
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
                if (obstacles[i].x >= x_grid && obstacles[i].y >= y_grid) {
                    cout << "error in input file!! Out of range index (obstacles)!!\n";
                    return -1;
                }
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

                    if (nets[j][i].y >= x_grid && nets[j][i].y >= y_grid) {
                        cout << "error in input file!! Out of range index (nets)!!\n";
                        return -1;
                    }
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

    obs_visual = obstacles;
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
    return 1;
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
    /*
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
    */


    // Printing matrix

    //for (int i = 0; i < x_grid; i++)
    //{
    //    for (int j = 0; j < y_grid; j++)
    //    {
    //        if (matrix[i][j] == INT_MAX)
    //            cout << setw(10) << "O";
    //        else
    //            cout << setw(10) << matrix[i][j];
    //    }
    //    cout << endl;
    //}

    //cout << endl;


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

obj source_knowing_target(obj target, vector <vector <int>> matrix)
{
    int x = target.x;
    int y = target.y;
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
        x = x_path; y = y_path;
    }
    obj src(x, y, 1, target.net_name); //Default metal layer
    return src;
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

vector <obj> test1(obj src, obj tar) {
    vector <obj> path = shortest_path(src, tar, lee(src, tar, src.M, 0));
    vector <bool> boolean = right_wrong(path, src.M);
    comparing(boolean, path);
    return path;

}

vector <obj> test2(obj src, obj tar) {

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

        it = path.begin() + 1;
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

    return path;

}

vector <obj> detect_case(obj src, obj tar) {
    vector <obj> path;
    if (src.M == tar.M)
        path = test1(src, tar);
    else
        path = test2(src, tar);
    return path;
}


// For detecting the closest target
vector <obj> source_target(vector <obj> nets) {
    vector <obj> src_tar(2);
    src_tar[0] = nets[0];
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

    matrix[nets[0].x][nets[0].y] = 0;
    for (int i = 1; i < nets.size(); i++)
        matrix[nets[i].x][nets[i].y] = 100000;


    //Applying lee's algorithm
    int move_x[] = { -1, 0, 1, 0 };
    int move_y[] = { 0, 1, 0, -1 };

    queue<int> X, Y;

    X.push(nets[0].x);
    Y.push(nets[0].y);
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
                {
                    reached = true;
                    for (int p = 1; p < nets.size(); p++)
                        if (xx == nets[p].x && yy == nets[p].y)
                        {
                            src_tar[1] = nets[p];
                            break;
                        }
                    break;
                }
                if (visited[xx][yy] != -1 && visited[xx][yy] != -2 && matrix[xx][yy] != 100000)
                {
                    X.push(xx);
                    Y.push(yy);
                    visited[xx][yy] = -1;
                    if (nets[0].M == 1) // Horizontal (xx - current_x = 0)
                    {
                        if (xx == current_x)
                            matrix[xx][yy] = matrix[current_x][current_y] + 1;
                        else
                            matrix[xx][yy] = matrix[current_x][current_y] + wrong_cost;
                    }
                    else if (nets[0].M == 2) //Vertical (yy - current_y = 0)
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
    return src_tar;
}


// Multiple sources ---> Determining the closest target
vector <obj> lee_multiple_sources(vector <obj> path, vector <obj> nets) {
    //cout << path.size() << "  " << nets.size() << endl;
    vector <obj> src_tar(2);
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

    queue<int> Xi, Yi;
    for (int i = 0; i < path.size(); i++)
    {
        visited[path[i].x][path[i].y] = -1;
        matrix[path[i].x][path[i].y] = 0;
        Xi.push(path[i].x);
        Yi.push(path[i].y);
    }

    for (int i = 0; i < nets.size(); i++)
        matrix[nets[i].x][nets[i].y] = 100000;


    //Applying lee's algorithm
    int move_x[] = { -1, 0, 1, 0 };
    int move_y[] = { 0, 1, 0, -1 };

    int current_x, current_y, xx, yy;
    bool reached = false;
    while (!Xi.empty() && !reached)
    {
        current_x = Xi.front();
        current_y = Yi.front();

        visited[current_x][current_y] = -1;
        for (int i = 0; i < 4; i++)
        {
            xx = current_x + move_x[i];
            yy = current_y + move_y[i];

            if (xx >= 0 && yy >= 0 && xx < x_grid && yy < y_grid)
            {
                if (matrix[xx][yy] == 100000)
                {
                    reached = true;
                    for (int p = 0; p < nets.size(); p++)
                        if (xx == nets[p].x && yy == nets[p].y)
                        {

                            // Printing matrix
                          /*  cout << "Multiple source matrix" << endl;
                            for (int z = 0; z < matrix.size(); z++)
                            {
                                for (int x = 0; x < matrix[z].size(); x++)
                                {
                                    if (matrix[z][x] == INT_MAX)
                                        cout << setw(10) << "O";
                                    else
                                        cout << setw(10) << matrix[z][x];
                                }
                                cout << endl;
                            }
                            cout << endl;*/

                            src_tar[1] = nets[p];
                            src_tar[0] = source_knowing_target(nets[p], matrix);
                            for (int y = 0; y < path.size(); y++)
                                if (path[y].x == src_tar[0].x && path[y].y == src_tar[0].y)
                                {
                                    src_tar[0] = path[y];
                                    break;
                                }
                            break;
                        }
                    break; // Testing extra break
                }
                if (visited[xx][yy] != -1 && visited[xx][yy] != -2 && matrix[xx][yy] != 100000)
                {
                    Xi.push(xx);
                    Yi.push(yy);
                    visited[xx][yy] = -1;
                    if (nets[0].M == 1) // Horizontal (xx - current_x = 0)
                    {
                        if (xx == current_x)
                            matrix[xx][yy] = matrix[current_x][current_y] + 1;
                        else
                            matrix[xx][yy] = matrix[current_x][current_y] + wrong_cost;
                    }
                    else if (nets[0].M == 2) //Vertical (yy - current_y = 0)
                    {
                        if (yy == current_y)
                            matrix[xx][yy] = matrix[current_x][current_y] + 1;
                        else
                            matrix[xx][yy] = matrix[current_x][current_y] + wrong_cost;
                    }
                }
            }
        }

        Xi.pop();
        Yi.pop();
    }

    // cout << "Source : " << src_tar[0].x << " " << src_tar[0].y << endl;
    // cout << "Target : " << src_tar[1].x << " " << src_tar[1].y << endl;
    return src_tar;
}


vector <vector <obj>> reading_nets() {

    for (int i = 0; i < nets.size(); i++) {
        vector <obj> src_tar = source_target(nets[i]);
        obj src = src_tar[0];
        obj tar = src_tar[1];

        vector <obj> path = detect_case(src, tar);

        vector <obj>::iterator it;
        it = nets[i].begin();
        for (int l = 0; l < nets[i].size(); l++)
            if (tar.x == nets[i][l].x && tar.y == nets[i][l].y && tar.M == nets[i][l].M)
            {
                nets[i].erase(it + l);
                break;
            }
        nets[i].erase(it);

        while (!nets[i].empty())
        {
            src_tar = lee_multiple_sources(path, nets[i]);
            obj src = src_tar[0];
            obj tar = src_tar[1];
            if (tar.net_name == "") {
                break;
            }
            vector <obj> cont = detect_case(src, tar);
            cont.pop_back();
            it = nets[i].begin();
            for (int l = 0; l < nets[i].size(); l++)
                if (tar.x == nets[i][l].x && tar.y == nets[i][l].y && tar.M == nets[i][l].M)
                {
                    nets[i].erase(it + l);
                    break;
                }
            path.insert(path.begin(), cont.begin(), cont.end());
            final_path.push_back(path);
            obstacles.insert(obstacles.end(), path.begin(), path.end());
        }
    }
    return final_path;
}

void visualizing(vector<vector<obj>> paths) {

    //empty cells
    vector<vector<int>>out(y_grid);
    for (int i = 0; i < y_grid; i++)
        for (int j = 0; j < x_grid; j++)
            out[i].push_back(0);


    //nets
    int counter = 3;
    for (int i = 0; i < paths.size(); i++) {
        counter++;
        for (int j = 0; j < paths[i].size(); j++) {
            out[i][j] = counter;
        }
    }

    ////intersections between nets
    //for(int a = 0; a < paths.size(); a++){
    //    for (int i = 0; i < paths[a].size(); i++) {
    //        for (int j = a + 1; j < paths.size(); j++) {
    //            for (int k = 0; k < paths[j].size(); k++) {

    //                if (paths[a][i].x == paths[j][k].x && paths[a][i].y == paths[j][k].y) {

    //                    out[paths[a][i].x][paths[a][i].y] = 3;
    //                }

    //            }
    //        }
    //    }
    //}

    //via cells
    for (int i = 0; i < paths.size(); i++) {
        for (int j = 1; j < paths[i].size(); j++) {
            if (paths[i][j - 1].x == paths[i][j].x && paths[i][j - 1].y == paths[i][j].y) {
                out[paths[i][j - 1].x][paths[i][j - 1].y] = 2;
            }
        }
    }

    //obstacles cells
    for (int i = 0; i < obs_visual.size(); i++) {
        out[obs_visual[i].x][obs_visual[i].y] = 1;
    }

    cout << endl;
    for (int i = 0; i < y_grid; i++) {
        for (int j = 0; j < x_grid; j++) {
            cout << out[i][j] << " ";
        }
        cout << endl;
    }
}
void printing(vector <vector <obj>> path) {

    for (int i = 0; i < path.size(); i++) {
        cout << endl << path[i][0].net_name << " : ";
        for (int j = path[i].size() - 1; j >= 0; j--)
            cout << "(" << path[i][j].M << " , " << path[i][j].x << " , " << path[i][j].y << ")" << "  ";
        cout << endl;
        cout << endl;
    }

}


int main() {
    if (init_file() != -1) {
        resizing();
        printing(reading_nets());
    }
    //visualizing(final_path);

    return 0;
}