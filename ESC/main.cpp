#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include "mouseInput.cpp"
#define GL_LOG_FILE "/Users/chaidhatchaimongkol/Documents/ESC/ESC/ESC/gl.log"
#define GL_TEXTURE_FILE "/Users/chaidhatchaimongkol/Documents/ESC/ESC/ESC/gl.bmp"

bool restart_gl_log() {
    FILE* file = fopen(GL_LOG_FILE, "w");
    if(!file) {
        fprintf(stderr,
                "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
                GL_LOG_FILE);
        return false;
    }
    else
    {
    }
    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
    fclose(file);
    return true;
}

bool gl_log(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file) {
        fprintf(
                stderr,
                "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE
                );
        return false;
    }
    printf(message, argptr);
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool gl_log_err(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file) {
        fprintf(stderr,
                "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

void glfw_error_callback(int error, const char* description) {
    gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

//GLuint loadBMP_custom(const char * imagepath);
//GLuint image = loadBMP_custom("./my_texture.bmp");

// Data read from the header of the BMP file
unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;

// Open the file
GLuint load () {
    
    FILE * file = fopen(GL_TEXTURE_FILE,"rb");
    if (!file){printf("Image could not be opened\n"); return 0;}

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return GL_TEXTURE_2D;
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

double xMouse;
double yMouse;
double yMo = 0;
double yMoo = 0;
glm::vec3 playerTransform = glm::vec3(4,3,3);
glm::vec3 playerVelocity = glm::vec3(0);
glm::vec3 playerAccel = glm::vec3(0);

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    xMouse = xpos / -100;
    //if (sinf(((-yMouse-4.5)/4.5)*180*0.01745329252) > -0.99)
        //yMo += 1;
    if (ypos - yMoo < yMo)
    {
        yMoo = ypos - yMo;
        yMouse = ((ypos - yMoo) / -100);
    }
    else
    {
        if ((cosf(((-(ypos - yMoo) / -100)-4.5)/4.5)*180*0.01745329252) > 0)
        {
            yMo = -99999;
            yMouse = ((ypos - yMoo) / -100);
        }
        else
        {
            if (yMo == -99999)
                yMo = ypos - yMoo;
        }
    }
}


int main() {
    printf("%f", mouseIn::Input());
    assert(restart_gl_log());
    // start GL context and O/S window using the GLFW helper library
    gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback(glfw_error_callback);
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Hello Triangle", mon, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("\nRenderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -1.f,  0.0f,
    };
    
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "layout(location = 0) in vec3 vertexPosition_modelspace;"
    ""
    "uniform mat4 MVP;"
    ""
    "void main(){"
     "   gl_Position =  MVP * vec4(vertexPosition_modelspace,1.0);"
    "}";
    
    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 4 / (float)3, 0.1f, 100.0f);
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    glClearColor(0, 0, 0.1, 1);
    bool init = false;
    
    while(!glfwWindowShouldClose(window)) {
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
        
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
            playerAccel = glm::vec3(sinf(((-xMouse-7)/7)*180*0.01745329252), 0, cosf(((-xMouse-7)/7)*180*0.01745329252));
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
                playerAccel = glm::vec3(-sinf(((-xMouse-7)/7)*180*0.01745329252), 0, -cosf(((-xMouse-7)/7)*180*0.01745329252));
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
            playerAccel = glm::vec3(-cosf(((-xMouse-7)/7)*180*0.01745329252), 0, -sinf(((-xMouse-7)/7)*180*0.01745329252));
        }
        else
        {
            playerAccel = playerVelocity * glm::vec3(-0.1);
        }
        playerVelocity = playerVelocity + (playerAccel / glm::vec3(1));
        
        
        /*if (playerVelocity.x > 0.1)
        {
            playerVelocity.x -= 1;
        }
        else
        {
            if (playerVelocity.x < 0.1)
            {
                playerVelocity.x += 1;
            }
        }*/
        
        playerTransform = playerTransform + (playerVelocity / glm::vec3(1000));
        
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        
        float yl = sinf(((yMouse-4.5)/4.5)*180*0.01745329252);
        float zl = 1;
        if (cosf(((-yMouse-4.5)/4.5)*180*0.01745329252) > 0)
        {
            zl = cosf(((-yMouse-4.5)/4.5)*180*0.01745329252);
        }
        else
        {
        }
        
        //cosf(((yMouse-4.5)/4.5)*180*0.01745329252) + 3
        
        // Camera matrix
        glm::mat4 View = glm::lookAt(
                                     playerTransform, // Camera is at (4,3,3), in World Space
                                     //glm::vec3(sinf(((xMouse-7)/7)*180*0.01745329252) + 4, 3, cosf(((xMouse-7)/7)*180*0.01745329252) + 3), // and looks at the origin
                                     glm::vec3((sinf(((-xMouse-7)/7)*180*0.01745329252) * zl), yl, (cosf(((-xMouse-7)/7)*180*0.01745329252) * zl)) + playerTransform, // and looks at the origin
                                     glm::vec3(0,0.5,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                     );
        
        gl_log("\nx %f %f\n", yMouse * 100, sinf(((yMouse-4.5)/4.5)*180*0.01745329252));
        
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
        
        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        GLuint MatrixID = glGetUniformLocation(shader_programme, "MVP");
        
        glfwSetCursorPosCallback(window, cursor_position_callback);
        
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        
        if (!init)
        {
            init = true;
            gl_log("\nInitialised Successfully. %s %s \n", __DATE__, __TIME__);
        }
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

