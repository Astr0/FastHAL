using System;
using System.IO;

namespace FastDuino.Parser
{
    internal class Program
    {
        private static void GenerateFile(string inputFileName)
        {
            var input = File.ReadAllText(inputFileName);
            var output = DuinoConverter.Convert(input);
            var outputFileName = Path.Combine(Path.GetDirectoryName(inputFileName), "pins_fastduino.hpp");
            File.WriteAllText(outputFileName, output);
        }

        private static void GenerateDir(string dir)
        {
            Console.WriteLine("Generating {0}", dir);
            var pinsFile = Path.Combine(dir, "pins_arduino.h");
            if (File.Exists(pinsFile))
                GenerateFile(pinsFile);
            var subdirs = Directory.EnumerateDirectories(dir);
            foreach (var subdir in subdirs)
                GenerateDir(subdir);
        }

        private static void Main()
        {
            GenerateDir(".");

            Console.WriteLine("Done!");
            Console.ReadLine();
        }
    }
}