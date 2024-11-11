idf_component_register(SRCS "main.c" "tasks.c" "tasks_orquestor.c"
                    INCLUDE_DIRS "." "include"
                    REQUIRES "dron")
list( APPEND EXTRA_COMPONENTS_DIRS components )