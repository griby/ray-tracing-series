
#include <iostream>
#include <fstream>

const std::string IMAGE_FILE_PATH = "assets/simple_image.ppm";

int main()
{
    std::cout << "Ray Tracing in One Weekend Book Series by Peter Shirley" << std::endl;

    std::ofstream imageFile(IMAGE_FILE_PATH);
    if (imageFile.is_open())
    {
        int nx = 200;
        int ny = 100;

        imageFile << "P3" << std::endl;
        imageFile << nx << " " << ny << std::endl;
        imageFile << "255" << std::endl;

        // From top to bottom
        for (int j = ny - 1; j >= 0; --j)
        {
            // From left to right
            for (int i = 0; i < nx; ++i)
            {
                float r = float(i) / float(nx);
                float g = float(j) / float(ny);
                float b = 0.2f;

                int ir = int(255.99f * r);
                int ig = int(255.99f * g);
                int ib = int(255.99f * b);

                imageFile << ir << " " << ig << " " << ib << std::endl;
            }
        }

        imageFile.close();
    }

    return 0;
}
