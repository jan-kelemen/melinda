build_flatbuffers(
    "wire.fbs" # Schema files
    "" # Schema include directories 
    "fb" # Target name 
    "" # Additional dependencies
    "${CMAKE_CURRENT_SOURCE_DIR}" # Generated includes directory
    "" # Binary schema directory
    "" # Text schemas directory
)

