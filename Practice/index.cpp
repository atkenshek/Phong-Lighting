#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



GLuint vbo, vbo1, posloc, normloc, trloc, prloc, shp;
glm::mat4 transf;
std::vector<float>  verts;
std::vector<glm::vec3>  normals;



void display()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, verts.size() / 3);



    glutSwapBuffers();
}



bool leftdown = false;



void idle()
{
    if(leftdown)
    transf = glm::rotate(transf, 0.02f, glm::vec3(0, 0, 1)); //changed this one



    glm::mat4 proj = glm::perspective(45.f, 1.f, 0.01f, 100.f);
    glm::mat4 lookat = glm::lookAt(glm::vec3(4, 4, 0), glm::vec3(), glm::vec3(-1, 0, 1));
    proj = proj * lookat;
    //proj = glm::mat4(1);
    glUniformMatrix4fv(trloc, 1, GL_FALSE, glm::value_ptr<float>(transf));
    glUniformMatrix4fv(prloc, 1, GL_FALSE, glm::value_ptr<float>(proj));
    glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        leftdown = true;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftdown = false;
    //transf = glm::translate(transf, glm::vec3(0, 0.001, 0));
    //transf = glm::scale(transf, glm::vec3(0.999, 0.999, 0));
}



void motion(int x, int y)
{



}



void keyboard(unsigned char key, int, int)
{



}



int main(int argc, char** argv) {
    transf = glm::mat4(1);



    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(argv[0]);

    glewInit();



    shp = glCreateProgram();
    GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vshCode =
        "#version 150 \n"
        "attribute vec3 pos; \n"
        "attribute vec3 normal; \n"
        "uniform mat4 transformation; \n"
        "uniform mat4 projection; \n"
        "varying float diffuse; \n"
        "void main(){"
        "vec3 lpos = vec3(2,2,2);"
        "diffuse = dot(normalize(normal), normalize(lpos-pos) );"
        "gl_Position = projection * transformation * vec4(pos, 1);}";
    GLint vshlen = strlen(vshCode);
    glShaderSource(vsh, 1, &vshCode, &vshlen);
    glAttachShader(shp, vsh);
    glCompileShader(vsh);



    GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fshCode =
        "#version 150 \n"
        "varying float diffuse; \n"
        "void main(){"
        "gl_FragColor = vec4(diffuse, 0, 0, 1);}";
    GLint fshlen = strlen(fshCode);
    glShaderSource(fsh, 1, &fshCode, &fshlen);
    glAttachShader(shp, fsh);
    glCompileShader(fsh);



    glValidateProgram(shp);
    glLinkProgram(shp);
    glUseProgram(shp);




    auto pi = acos(-1.0);
    for (int i = 0; i < 4; ++i)
    {

        verts.push_back(0);
        verts.push_back(0);
        verts.push_back(2);


        verts.push_back(cos(i * 90.0 * pi / 180.0));
        verts.push_back(sin(i * 90.0 * pi / 180.0));
        verts.push_back(0);

        verts.push_back(cos((i + 1) * 90.0 * pi / 180.0));
        verts.push_back(sin((i + 1) * 90.0 * pi / 180.0));
        verts.push_back(0);


        glm::vec3 D = glm::vec3(verts[verts.size() - 3], verts[verts.size() - 2],
            verts[verts.size() - 1]);
        glm::vec3 E = glm::vec3(verts[verts.size() - 6], verts[verts.size() - 5],
            verts[verts.size() - 4]);
        glm::vec3 F = glm::vec3(verts[verts.size() - 9], verts[verts.size() - 8],
            verts[verts.size() - 7]);



        auto d = glm::normalize(D - F);
        auto e = glm::normalize(E - F);



        auto f = glm::cross(d, e);



        for (int j = 0; j < 3; ++j)
        {
            normals.push_back(f);
        }


        verts.push_back(0);
        verts.push_back(0);
        verts.push_back(-2);

        verts.push_back(cos(i * 90.0 * pi / 180.0));
        verts.push_back(sin(i * 90.0 * pi / 180.0));
        verts.push_back(0);

        verts.push_back(cos((i + 1) * 90.0 * pi / 180.0));
        verts.push_back(sin((i + 1) * 90.0 * pi / 180.0));
        verts.push_back(0);



        glm::vec3 A = glm::vec3(verts[verts.size() - 3], verts[verts.size() - 2],
            verts[verts.size() - 1]);
        glm::vec3 B = glm::vec3(verts[verts.size() - 6], verts[verts.size() - 5],
            verts[verts.size() - 4]);
        glm::vec3 C = glm::vec3(verts[verts.size() - 9], verts[verts.size() - 8],
            verts[verts.size() - 7]);



        auto a = glm::normalize(A - C);
        auto b = glm::normalize(B - C);



        auto n = glm::cross(b, a);



        for (int j = 0; j < 3; ++j)
        {
            normals.push_back(n);
        }
    }




    for (int i = 0; i < normals.size() / 6; ++i)
    {
        if (i != 0 && i != normals.size() - 1)
        {
            auto n = 0.25f * (normals[6 * i - 5] + normals[6 * i + 2] + normals[6 * i - 1] + normals[6 * i + 1]);
            normals[6 * i - 5] = n;
            normals[6 * i + 2] = n;
            normals[6 * i + 1] = n;
            normals[6 * i - 1] = n;
        }
    }
    auto n = 0.25f * (normals[normals.size() - 5] + normals[normals.size() - 2] + normals[2] + normals[5]);
    normals[normals.size() - 5] = n;
    normals[normals.size() - 2] = n;
    normals[2] = n;
    normals[5] = n;
    




    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    posloc = glGetAttribLocation(shp, "pos");
    glVertexAttribPointer(posloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posloc);

    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

    normloc = glGetAttribLocation(shp, "normal");
    glVertexAttribPointer(normloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normloc);

    trloc = glGetUniformLocation(shp, "transformation");
    prloc = glGetUniformLocation(shp, "projection");



    glEnable(GL_DEPTH_TEST);



    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);



    glutMainLoop();
    return 0;
}