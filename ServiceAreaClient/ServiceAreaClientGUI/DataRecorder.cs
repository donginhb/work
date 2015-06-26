using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using System.Threading;

namespace UIManager
{
    class DataRecorder
    {
        List<string> _bufList = new List<string>();
        int _maxCount = 1000;

        public int MaxCount
        {
            get { return _maxCount; }
            set { _maxCount = value; }
        }

        public DataRecorder(int maxCount = 1000)
        {
            MaxCount = maxCount;
        }

        // TODO: 以后这些方法都要改成异步的
        public string Add(string addStr)
        {
            string timeStamp = Common.GetTimeStamp();
            _bufList.Add(timeStamp + "," + addStr);
            if (_bufList.Count >= MaxCount)
            {
                Save2File();
            }
            return timeStamp;
        }

        public void Save2File()
        {
            if (0 == _bufList.Count)
            {
                return;
            }
            Thread sfThd = new Thread(new ThreadStart(SaveDataStart));
            sfThd.Start();
        }

        void SaveDataStart()
        {
            string fname = GetSaveFilePath();
            StreamWriter sw = new StreamWriter(fname, true);
            foreach (string dataStr in _bufList)
            {
                sw.WriteLine(dataStr);
            }
            sw.Close();
            _bufList.Clear();
        }

        string GetSaveFilePath()
        {
            string exePath = System.Windows.Forms.Application.ExecutablePath;
            int idx = exePath.LastIndexOf('\\');
            if (-1 != idx)
            {
                exePath = exePath.Remove(idx + 1);
            }
            string fPath = exePath + "data\\" + DateTime.Now.Year.ToString() + "\\"
                            + DateTime.Now.Month.ToString() + "\\" + DateTime.Now.Day.ToString() + "\\";
            if (!Directory.Exists(fPath))
            {
                Directory.CreateDirectory(fPath);
            }
            DirectoryInfo di = new DirectoryInfo(fPath);
            FileInfo[] fiList = di.GetFiles();
            int fileCount = 0;
            foreach (FileInfo fi in fiList)
            {
                string fn = fi.Name.Remove(fi.Name.LastIndexOf('.')).Trim();
                int val;
                if (int.TryParse(fn, out val))
                {
                    if (val > fileCount)
                    {
                        fileCount = val;
                    }
                }
            }
            fileCount += 1;
            fPath += fileCount.ToString() + ".txt";
            return fPath;
        }
    }
}
