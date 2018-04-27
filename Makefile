SOURCE_DIR:=${shell dirname ${realpath ${lastword ${MAKEFILE_LIST}}}}
LLVM_SOURCE_DIR=${SOURCE_DIR}/external/llvm

BUILD_DIR := ${SOURCE_DIR}/build
LLVM_BUILD_DIR := ${BUILD_DIR}/llvm
PLAYGROUND_BUILD_DIR := ${BUILD_DIR}/playground

GIT := git
CMAKE := cmake

all: submodule-init llvm playground

dev: format configure-playground build-playground

submodule-init:
	${GIT} submodule update --init --recursive

llvm: configure-llvm build-llvm

configure-llvm:
	mkdir -p ${LLVM_BUILD_DIR}
	${CMAKE} \
		-H${LLVM_SOURCE_DIR} \
		-B${LLVM_BUILD_DIR} \
		-G Ninja \
		-DCMAKE_BUILD_TYPE=Debug \
		-DLLVM_ENABLE_ASSERTIONS=On \
		-DLLVM_TARGETS_TO_BUILD="X86"

build-llvm:
	${CMAKE} --build ${LLVM_BUILD_DIR}

playground: configure-playground build-playground

configure-playground:
	mkdir -p ${PLAYGROUND_BUILD_DIR}
	${CMAKE} \
		-H${SOURCE_DIR} \
		-B${PLAYGROUND_BUILD_DIR} \
		-DLLVM_CMAKE_CONFIG_PATH=${LLVM_BUILD_DIR}/lib/cmake/llvm \
		-G Ninja

build-playground:
	${CMAKE} --build ${PLAYGROUND_BUILD_DIR}

format:
ifdef CLANG_FORMAT
	${CLANG_FORMAT} -i \
		$(shell \
			find ${SOURCE_DIR} \( -name '*.h' -o -name '*.cpp' \) \
			-not -path '${SOURCE_DIR}/external/*' \
			-not -path '${BUILD_DIR}/*')
endif

