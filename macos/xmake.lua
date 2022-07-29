add_requires("glfw 3.3.8", "glad", "glm", "stb")

target("OpenGL-Training")
    set_kind("binary")
    set_languages("cxx17")
    add_files("src/**.cpp")
    add_packages("glfw", "glad", "glm", "stb")
    add_headerfiles("src/**.hpp")