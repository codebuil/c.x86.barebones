using System;

namespace os
{
    class oss
    {

		public static int adds(int a,int b){
			return a+b;
		}
	static void kernel_main()

        {
           	    string memory = "10+10={0} \n";
            Console.BackgroundColor = ConsoleColor.Blue;
            Console.Clear();
			Console.Write(memory,adds(10,10));
            

        }
        static void Main(string[] args)

        {
		kernel_main();
        }
    }
}

