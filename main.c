#include <cstdlib>
#include <cstdio>

#define MIRROR_45 0
#define MIRROR_135 1

typedef enum {NORTH = 0, EAST = 2, SOUTH = 4, WEST = 6} Direction;


inline Direction new_dir(Direction curr_dir, char sign){
    switch(curr_dir + (sign == '/' ? 0 : 1)){
        case NORTH + MIRROR_45: return EAST;
        case NORTH + MIRROR_135: return WEST;
        case EAST + MIRROR_45: return NORTH;
        case EAST + MIRROR_135: return SOUTH;
        case SOUTH + MIRROR_45: return WEST;
        case SOUTH + MIRROR_135: return EAST;
        case WEST + MIRROR_45: return SOUTH;
        default: return NORTH;
    }
}


inline void new_coord(int *x, int *y, Direction direction){
    switch(direction){
        case NORTH: --*x; break;
        case EAST: ++*y; break;
        case SOUTH: ++*x; break;
        default: --*y; break;
    }
}

bool solve(int x, int y, char **tab, int mirrors, Direction direction, int crystals){
    int new_x = x;
    int new_y = y;
    char tile = tab[x][y];

    if(tile == ' '){
        new_coord(&new_x, &new_y, direction);
        if(tab[new_x][new_y] != '#'){
            tab[x][y] = 'x';
            if(solve(new_x, new_y, tab, mirrors, direction, crystals)) {
                tab[x][y] = ' ';
                return true;
            }
        }

        if(mirrors > 0)
            for(char c: (char[]){'/', '\\'}){
                tab[x][y] = c;
                new_x = x;
                new_y = y;
                Direction dir = new_dir(direction, c);
                new_coord(&new_x, &new_y, dir);

                if(tab[new_x][new_y] == '#')
                    continue;
                if(solve(new_x, new_y, tab, mirrors-1, dir, crystals))
                    return true;
            }

        tab[x][y] = ' ';
        return false;
    }
    else if(tile == '*'){
        if(crystals == 1)
            return true;

        new_coord(&new_x, &new_y, direction);
        if(tab[new_x][new_y] == '#')
            return false;

        tab[x][y] = '^';
        bool tmp = solve(new_x, new_y, tab, mirrors, direction, crystals-1);
        tab[x][y] = '*';
        return tmp;
    }
    else if(tile == '/' || tile == '\\'){
        return false;
    }
    else { // tile in ['x', '^']
        new_coord(&new_x, &new_y, direction);
        return tab[new_x][new_y] == '#' ? false : solve(new_x, new_y, tab, mirrors, direction, crystals);
    }
}

int main() {
    int height, width, mirrors_count, crystals_count = 0;
    (void) scanf("%d %d\n%d\n", &height, &width, &mirrors_count);

    int line_width = width + 2; // line + \n + \0
    char *mem = (char *) malloc((size_t) (line_width * height));
    char *tab[height], *line = mem;
    for(int i = 0; i < height; ++i, line += line_width) {
        (void) fgets(tab[i] = line, line_width, stdin);
        for(int j = 0; j < width; ++j)
            if(line[j] == '*')
                ++crystals_count;
    }

    if(mirrors_count != 0 && crystals_count != 0)
        solve(1, 0, tab, mirrors_count, EAST, crystals_count);

    printf("%d %d\n%d\n", height, width, mirrors_count);
    for (int i = 0; i < height; ++i)
        printf("%s", tab[i]);

    return 0;
}
