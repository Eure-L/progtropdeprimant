#include "viewer.h"

#include <filesystem/resolver.h>

void render(Scene* scene, ImageBlock* result, std::string outputName, bool* done)
{
    if(!scene)
        return;

    clock_t t = clock();

    const Camera *camera = scene->camera();
    const Integrator* integrator = scene->integrator();
    integrator->preprocess(scene);

    float tanfovy2 = tan(camera->fovY()*0.5);
    Vector3f camX = camera->right() * tanfovy2 * camera->nearDist() * float(camera->vpWidth())/float(camera->vpHeight());
    Vector3f camY = -camera->up() * tanfovy2 * camera->nearDist();
    Vector3f camF = camera->direction() * camera->nearDist();

    /// TODO:
    ///  1. iterate over the image pixels
    ///  2. generate a primary ray
    ///  3. call the integartor to compute the color along this ray
    ///  4. write this color in the result image
    
    Point3f origin = camera->position();
    Point3f I;
    unsigned int width = camera->vpWidth();
    unsigned int height = camera->vpHeight();
    Vector3f vx = camX / (width/2);
    Vector3f vy = camY / (width/2); 

    Point3f target = origin + camF - camX - camY + vx/2 +vy/2;
    // D = I - camX - camY
    for(int pixx = 0; pixx < width; pixx++){
        for(int pixy = 0; pixy < height; pixy++){

            // direction du rayon
            Vector3f dx = (2*(pixy+0.5)*camX) / (width-1);
            Vector3f dy = (2*(pixx+0.5)*camY) / (height-1);
            Vector3f d = dx + dy + camF;
            d.normalize();

            Ray ray(origin,d);
            Color3f col = integrator->Li(scene,ray);

            result->put(Vector2f(pixx,pixy),col);
            target += vx;
        }
        target -= vx*width;
        target += vy;
    }

    Bitmap image = *result->toBitmap();
    image.save(outputName);
    
    t = clock() - t;
    std::cout << "Raytracing time : " << float(t)/CLOCKS_PER_SEC << "s"<<std::endl;

    *done = true;
}

int main(int argc, char *argv[])
{
    getFileResolver()->prepend(DATA_DIR);

    try {
        nanogui::init();
        Viewer *screen = new Viewer();;

        if (argc == 2) {
            /* load file from the command line */
            filesystem::path path(argv[1]);

            if(path.extension() == "scn") { // load scene file
                screen->loadScene(argv[1]);
            }else if(path.extension() == "exr") { // load OpenEXR image
                screen->loadImage(argv[1]);
            }
        }

        /* Enter the application main loop */
        nanogui::mainloop();

        delete screen;
        nanogui::shutdown();
    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
