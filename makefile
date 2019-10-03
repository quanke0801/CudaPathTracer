include_directory := ./include
source_directory := ./source
build_directory := ./build

source_files := $(wildcard $(source_directory)/*.cpp)
build_files := $(patsubst $(source_directory)/%.cpp,$(build_directory)/%.o,$(source_files))

target = tracer.exe

$(target): $(build_files)
	nvcc -arch=sm_61 $(build_files) -o $@

$(build_directory)/%.o: $(source_directory)/%.cpp
	nvcc -x cu -arch=sm_61 -lineinfo -dc -I $(include_directory) $< -o $@

clean:
	rm $(build_files) $(target)
