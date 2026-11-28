target_sources(
    requite
    PRIVATE
        build.cpp
        generate.cpp
        context.cpp
        emit_requite.cpp
        options.cpp
        parse.cpp
        situate.cpp
        tokenize.cpp
)

target_sources(
    requitec
    PRIVATE
        main.cpp
)
