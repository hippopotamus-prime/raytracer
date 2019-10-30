#include <iostream>
#include "NFF.h"
#include "Scene.h"

using namespace std;

int main(int argc, char *argv[])
{
    Scene scene;
    ReadNFF(scene, cin);

    unsigned char output[scene.height * scene.width * 3];

    scene.Prepare();
    scene.Render(output);

    FILE *f = fopen("trace.ppm", "wb");
    fprintf(f, "P6\n%d %d\n%d\n", scene.width, scene.height, 255);
    size_t count = scene.height * scene.width * 3;
    if(fwrite(output, 1, count, f) < count)
    {
        cerr << "Error: unable to write to file \"trace.ppm\"" << endl;
    }
    fclose(f);

    return 0;
}
