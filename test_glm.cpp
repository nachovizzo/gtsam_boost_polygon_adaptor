#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>

typedef std::vector< glm::vec2 > Poly;
void drawPoly( const Poly& poly )
{
    if( poly.size() == 1 )
        glBegin( GL_POINTS );
    else
        glBegin( GL_LINE_STRIP );

    for( const auto& pt : poly )
    {
        glVertex2f( pt.x, pt.y );
    }

    glEnd();
}

typedef std::vector< Poly > Polys;
Polys polys( 1 );
void mouse( int button, int state, int x, int y )
{
    if( GLUT_UP == state && GLUT_LEFT_BUTTON == button )
    {
        polys.back().push_back( glm::vec2( x, y ) );
        glutPostRedisplay();
    }
    if( GLUT_UP == state && GLUT_RIGHT_BUTTON == button )
    {
        polys.back().push_back( polys.back().front() );
        polys.push_back( Poly() );
        glutPostRedisplay();
    }
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    double w = glutGet( GLUT_WINDOW_WIDTH );
    double h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, h, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glColor3ub( 255, 255, 255 );
    for( const auto& poly : polys )
    {
        drawPoly( poly );
    }

    glutSwapBuffers();
}

int main( int argc, char **argv )
{
    glutInitWindowSize( 640, 480 );
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "GLUT" );
    glutDisplayFunc( display );
    glutMouseFunc( mouse );
    glutMainLoop();
    return 0;
}
