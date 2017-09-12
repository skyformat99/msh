#define MSH_DRAW_IMPLEMENTATION
#define TT_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#include "msh.h"
#include "glad/glad.h"
#include "tt/tiny_time.h"
#include "stb/stb_image.h"
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h" //TODO(maciej): This is temp
#include "msh_draw.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


#include "stdio.h"

int main( int argc, char** argv )
{
  GLFWwindow* window;
  msh_draw_ctx_t draw_ctx;

  // Initialize GLFW and create window
  if( !glfwInit() )
  {
    msh_eprintf("Could not initialize glfw!\n");
    exit(EXIT_FAILURE);
  } 

  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  glfwWindowHint( GLFW_SAMPLES, 1 );
  // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
  window = glfwCreateWindow( 1024, 512, "Simple example", NULL, NULL );

  if( !window )
  {
    msh_eprintf("Could not open window :<!\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);

  // Initialize context
  if( !msh_draw_init_ctx( &draw_ctx ) )
  {
    msh_eprintf("Could not initialize draw context!\n");
  }

  // Load images
  printf("Starting loading\n");
  int img_width, img_height, img_n_channels;

  unsigned char* kitten = stbi_load( "data/kitten.jpg", &img_width, &img_height, &img_n_channels, 3);
  const int kitten_idx = msh_draw_register_image( &draw_ctx, kitten, img_width, img_height, img_n_channels );
  
  unsigned char* seal = stbi_load( "data/seal.jpg", &img_width, &img_height, &img_n_channels, 3);
  const int seal_idx = msh_draw_register_image( &draw_ctx, seal, img_width, img_height, img_n_channels );
 

  unsigned char* puppy = stbi_load( "data/puppy.jpg", &img_width, &img_height, &img_n_channels, 3);
  const int puppy_idx = msh_draw_register_image( &draw_ctx, puppy, img_width, img_height, img_n_channels );
 
  //TODO(maciej): Add font buffer
  int font_paint = msh_draw_add_font( &draw_ctx, "data/raleway.ttf", 62 );


  stbi_image_free(kitten);
  stbi_image_free(seal);
  stbi_image_free(puppy);
  

  // Draw loop
  float y1 = 0.0f;
  float y2 = 0.0f;

  GLuint query;
  GLuint64 elapsed_time;
  glGenQueries(1, &query);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glBlendEquation(GL_FUNC_ADD);
  // glEnable(GL_FRAMEBUFFER_SRGB);

  while( !glfwWindowShouldClose(window) )
  {
    int done = 0;
    int fb_w, fb_h;
    double mx, my;

    glfwGetFramebufferSize( window, &fb_w, &fb_h );
    glfwGetCursorPos(window, &mx, &my);
    
    glViewport( 0, 0, fb_w, fb_h );
    glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBeginQuery(GL_TIME_ELAPSED,query);
    elapsed_time = 0;
    ttTime();
    msh_draw_new_frame( &draw_ctx, fb_w, fb_h );
    
    // Add paints
    // TODO(maciej): Add hashing for paints ?
    // TODO(maciej): API for building paints.
    const int lin = msh_draw_linear_gradient_fill( &draw_ctx, 0.1f, 0.21f, 0.83f, 1.0f,
                                                              0.21f, 0.83f, 0.1f, 1.0f );
    const int pol = msh_draw_polar_gradient_fill( &draw_ctx, 0.1f, 0.21f, 0.83f, 1.0f,
                                                              0.21f, 0.83f, 0.1f, 1.0f );
    const int rad = msh_draw_radial_gradient_fill( &draw_ctx, 0.1f, 0.21f, 0.83f, 1.0f,
                                              0.21f, 0.83f, 0.1f, 1.0f,   
                                              256.0f, 0.0f );
    // TODO(maciej): Box parameters seem counter intuitive. Need to investigate
    const int box = msh_draw_box_gradient_fill( &draw_ctx, 0.1f, 0.21f, 0.83f, 1.0f,
                                           0.21f, 0.83f, 0.1f, 0.0f,
                                           32.0f, 16.0f, 16.0f );
    const int seal_img = msh_draw_image_fill( &draw_ctx, seal_idx );
    const int kitten_img = msh_draw_image_fill( &draw_ctx, kitten_idx );
    const int puppy_img = msh_draw_image_fill( &draw_ctx, puppy_idx );

    const int shadow = msh_draw_box_gradient_fill( &draw_ctx, 0.2f, 0.2f, 0.2f, 1.0f,
                                                              0.8f, 0.8f, 0.8f, 0.0f,
                                                              16.0f, 8.0f, 2.0f );

    msh_draw_set_paint( &draw_ctx, lin );
    msh_draw_rectangle( &draw_ctx, 64.0f, 64.0f, 128.0f, 256.0f);
    msh_draw_set_paint( &draw_ctx, rad );
    msh_draw_rectangle( &draw_ctx, 128.0f, 64.0f, 192.0f, 256.0f);
    msh_draw_set_paint( &draw_ctx, box );
    msh_draw_rectangle( &draw_ctx, 192.0f, 64.0f, 256.0f, 256.0f);
    msh_draw_set_paint( &draw_ctx, pol );
    msh_draw_rectangle( &draw_ctx, 256.0f, 64.0f, 320.0f, 256.0f);
    msh_draw_circle( &draw_ctx, 256.0f, 256.0f, 128.0f );
    msh_draw_set_paint( &draw_ctx, kitten_img );
    msh_draw_rectangle( &draw_ctx, 512.0f, 128.0f, 512.0f+128.0f, 256 );
    msh_draw_set_paint( &draw_ctx, puppy_img );
    msh_draw_rectangle( &draw_ctx, 512+128.0f, 128.0f, 512.0f+256.0f, 256 );
    msh_draw_set_paint( &draw_ctx, seal_img );
    msh_draw_rectangle( &draw_ctx, 512.0f+256.0f, 128.0f, 512+128.0f+256.0f, 256 );
    
    msh_draw_set_paint( &draw_ctx, shadow )
    msh_draw_set_paint( &draw_ctx, shadow );
    msh_draw_rectangle( &draw_ctx, 128.0f-8.0f, 128.0f-8.0f, 256.0f + 8.0f, 256.0f + 8.0f );
    msh_draw_set_paint( &draw_ctx, seal_img );
    msh_draw_rectangle( &draw_ctx, 128.0f, 128.0f, 256.0f, 256.0f );
    //TODO(maciej): Add string buffer
    int test = rand();
    char buf[1024];
    sprintf( buf, "Formatting test: %d\n", test );
    msh_draw_text(&draw_ctx, 512.0f, 390.0f, buf, font_paint );
    // Draw stuff
    // TODO(maciej): Investigate why number of draw calls inceases. Probably due to buffer limit.
    // for( int i = 0; i < 512; ++i )
    // {
    //   msh_draw_set_paint( &draw_ctx, i % 3 + 1 );
    //   msh_draw_rectangle( &draw_ctx, 64.0f + i*64.0f, 64.0f, 128.0f + i*64.0f, 256.0f );
    // }
    // msh_draw_rectangle( &draw_ctx, 64.0f, 256.0f, 256.0f, 320.0f );
    // msh_draw_set_paint( &draw_ctx, 1 );
    // msh_draw_rectangle( &draw_ctx, 256.0f, 256.0f, 448.0f, 384.0f );
    
    // static float t = 0.0f;
    // t += 0.01f;
    // float size = 256.0f;

    // msh_draw_gradient( &draw_ctx, 0.91f, 0.02f, 0.25f, 0.21f, 0.84f, 0.32f);
    // msh_draw_gradient( &draw_ctx, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    // msh_draw_rectangle( &draw_ctx, 100.0f, 100.0f, 250.0f, 400.0f );

    // msh_draw_gradient( &draw_ctx, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    // msh_draw_rectangle( &draw_ctx, 220.0f, 100.0f, 400.0f, 400.0f );

    // msh_draw_gradient( &draw_ctx, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    // msh_draw_rectangle( &draw_ctx, 450.0f, 100.0f, 514.0f, 164.0f );
    // msh_draw_gradient( &draw_ctx, 0.91f, 0.02f, 0.25f, 0.21f, 0.84f, 0.32f);
    // msh_draw_circle( &draw_ctx, 384.0f, 256.00f, size + 32 * sinf(t) );
    
    // msh_draw_triangle( &draw_ctx, 256.0f, 256.0f, size - 100 * sinf(t) );

    msh_draw_render( &draw_ctx );

    glEndQuery(GL_TIME_ELAPSED);
    while (!done) {
      glGetQueryObjectiv(query, 
                        GL_QUERY_RESULT_AVAILABLE, 
                        &done);
            }
    glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
    
    printf("Time Elapsed: %fs, %fms \n", ttTime(), elapsed_time/1000000.0f );
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 1;
}
