cmake_minimum_required(VERSION 3.0)
project(Cobra.io)

# Encontre o pacote OpenGL
find_package(OpenGL REQUIRED)
include_directories("C:/mingw/include")
include_directories("./lib")
# Localize o GLUT
set(GLUT_LIB_PATH "C:/mingw/lib/glut32.lib")
set(GLUT_INCLUDE_DIR "C:/mingw/include")

# Adicione seus arquivos de origem
add_executable(main main.cpp lib/game.hpp)

# Inclua o diretório de inclusão do GLUT
target_include_directories(main PRIVATE ${GLUT_INCLUDE_DIR})

# Especifique as bibliotecas que devem ser vinculadas
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIB_PATH})
