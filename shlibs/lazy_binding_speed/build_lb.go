package main

import (
	"fmt"
	"os"
	"os/exec"
	"strconv"
)

func main() {
	libPrefix := "lb_lib"
	mainName := "lb_main"
	makeFileName := "lb_Makefile"

	numLibs, _ := strconv.Atoi(os.Args[1])
	numFuncs, _ := strconv.Atoi(os.Args[2])

	targets := ""

	makeFile, _ :=  os.Create(makeFileName)

	mainFile, _ :=  os.Create(mainName + ".c")
	mainFile.WriteString(
		"#include <stdio.h>\n" +
		"#include <unistd.h>\n" +
		"#include <stdlib.h>\n")

	mfs := ""
	for  libNum := 1; libNum <= numLibs; libNum++ {
		libName := libPrefix + "_" + strconv.Itoa(libNum)
		libFile, _ :=  os.Create(libName + ".c")

		for funcNum := 1; funcNum <= numFuncs; funcNum++ {
			fsuffix := strconv.Itoa(libNum) + "_" +
				     strconv.Itoa(funcNum)
			funcName := "func_" + fsuffix

			libFile.WriteString(
				"int " + funcName + "(int j)\n" +
				"{\n" +
				"    return j + 1;\n" +
				"}\n" +
				"\n")

			mainFile.WriteString(
				"extern int " + funcName + "(int j);\n" +
				"\n" +
				"static void\n" +
				"call_" + funcName + "(int j)\n" +
				"{\n" +
				"    printf(\"Calling " + funcName +
				" returns %d\\n\", " +
				funcName +  "(j));\n" +
				"}\n" +
				"\n")
		}

		libFile.Close()

		mfs +=
			"\n" +
			libName + ".so: " + libName + ".c\n" +
			"\tcc -O0 -Wall -fPIC -shared -o " +
			libName + ".so " + libName  + ".c\n"

		targets += libName + ".so "
	}

	makeFile.WriteString(
		"all: " + mainName + " " + targets + "\n" +
		"\n" +
		mainName + ": " + mainName + ".o " + targets + "\n" +
		"\n" +
		"clean:\n" +
		"\trm lb_*\n" +
		"\trm " + makeFileName + "\n")

	makeFile.WriteString(mfs)

	mainFile.WriteString("int\n" +
		"main(int argc, char *argv[])\n" +
		"{\n" +
		"    exit(EXIT_SUCCESS);\n" +
		"}\n")

	mainFile.Close()

	exec.Command("make", "-j8", "-f", makeFileName).Output()
	fmt.Println("Done")

	os.Exit(0)
}
