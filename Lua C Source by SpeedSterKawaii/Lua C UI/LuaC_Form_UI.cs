using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lua_C_UI
{
    public partial class LuaC_Form_UI : Form
    {
        public void Attach()
        {
            if (NamedPipeExist(PipeName))
            {
                // The Pipe is Already Connected, No Need to Attach Again.
                MessageBox.Show(DLLName + " is Already Attached!", "Already attached!");
            }
            else
            {
                if (File.Exists(DLLName))
                {
                    // We add the dll into roblox.
                    this.AddDLLIntoRoblox(); // Add the DLL into the Roblox Process.
                }
                else
                {
                    // The DLLName was Not Found, PUT THE DLL THEN RETARD.
                    MessageBox.Show(DLLName + "couldn't be found!", "DLL not found!");
                }
            }
        }

        public void Execute(string lscript)
        {
            if (NamedPipeExist(PipeName))
            {
                // Pipe exist, we can send data (scripts) to the pipename.
                this.SendScriptData(lscript);
            }
            else
            {
                // The pipe doesnt exist, first we need to tell them to attach.
                MessageBox.Show("Please Attach before Executing the Script!", "Please attach!");
            }
        }

        public bool NamedPipeExist(string pipe)
        {
            // Why dont people just put this line? ITS SO DAMN EASY.
            return Directory.GetFiles(@"\\.\pipe\").Contains($@"\\.\pipe\{pipe}");
        }

        public static string PipeName = "LuaCPipe"; // This will be your lua pipe name.

        public static string DLLName = "Lua C Source.dll"; // This will be the DLL Name.

        private void SendScriptData(string script)
        {
            new Thread(delegate ()
            {
                try
                {
                    using (NamedPipeClientStream speedpipeclienstream = new NamedPipeClientStream(".", PipeName, PipeDirection.Out))
                    {
                        speedpipeclienstream.Connect();
                        using (StreamWriter streamWriter = new StreamWriter(speedpipeclienstream, Encoding.Default, 999999))
                        {
                            streamWriter.Write(script);
                            streamWriter.Dispose();
                        }
                        speedpipeclienstream.Dispose();
                        Thread.Sleep(10000);
                    }
                }
                catch (IOException)
                {
                    MessageBox.Show("IO Exception: Couldn't handle script execution.", "Connection Failed.");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString()); // Caught Error While Executing the Script to the 
                }
            }).Start();
        }

        private bool AddDLLIntoRoblox()
        {
            if (Process.GetProcessesByName("RobloxPlayerBeta").Length == 0) // Get Process "RobloxPlayerBeta".
            {
                return false;
            }
            Process process = Process.GetProcessesByName("RobloxPlayerBeta")[0];
            byte[] bytes = new ASCIIEncoding().GetBytes(AppDomain.CurrentDomain.BaseDirectory + DLLName); // Add the DLL into the Process RobloxPlayerBeta.exe
            IntPtr hModule = LoadLibraryA("kernel32.dll");
            UIntPtr procAddress = GetProcAddress(hModule, "LoadLibraryA");
            FreeLibrary(hModule);
            if (procAddress == UIntPtr.Zero)
            {
                return false;
            }
            IntPtr intPtr = OpenProcess(ProcessAccess.AllAccess, false, process.Id);
            if (intPtr == IntPtr.Zero)
            {
                return false;
            }
            IntPtr intPtr2 = VirtualAllocEx(intPtr, (IntPtr)0, (uint)bytes.Length, 12288u, 4u);
            UIntPtr uintPtr;
            IntPtr intPtr3;
            return !(intPtr2 == IntPtr.Zero) && WriteProcessMemory(intPtr, intPtr2, bytes, (uint)bytes.Length, out uintPtr) && !(CreateRemoteThread(intPtr, (IntPtr)0, 0u, procAddress, intPtr2, 0u, out intPtr3) == IntPtr.Zero);
        }

        [DllImport("kernel32", CharSet = CharSet.Ansi, SetLastError = true)]
        internal static extern IntPtr LoadLibraryA(string lpFileName);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        internal static extern UIntPtr GetProcAddress(IntPtr hModule, string procName);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr OpenProcess(ProcessAccess dwDesiredAccess, [MarshalAs(UnmanagedType.Bool)] bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", ExactSpelling = true, SetLastError = true)]
        internal static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        internal static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out UIntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, UIntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out IntPtr lpThreadId);

        [DllImport("kernel32.dll", SetLastError = true)]
        internal static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, [Out] byte[] lpBuffer, int dwSize, out int lpNumberOfBytesRead);

        [Flags]
        public enum ProcessAccess
        {
            AllAccess = 1050235,
            CreateThread = 2,
            DuplicateHandle = 64,
            QueryInformation = 1024,
            SetInformation = 512,
            Terminate = 1,
            VMOperation = 8,
            VMRead = 16,
            VMWrite = 32,
            Synchronize = 1048576
        }

    public LuaC_Form_UI()
        {
            InitializeComponent();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private async void LuaC_Form_UI_Load(object sender, EventArgs e)
        {
            while (true) 
            {
                await Task.Delay(555);
                if (NamedPipeExist("LuaCPipe")) // pipe name. 
                {
                    label2.ForeColor = Color.Green;
                    label2.Text = "DLL INJECTED";
                }
                else 
                {
                    label2.ForeColor = Color.Red;
                    label2.Text = "NOT INJECTED";
                }
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            Attach();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
