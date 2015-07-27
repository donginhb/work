using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
    /// <summary>
    /// 电表读数数据的设定类
    /// </summary>
    public static class ElectricMeterDataSetting
    {
		/// <summary>
		/// 电表读数设定类, 注意高字节在前
		/// </summary>
		public static List<DataUnitInfo> GetElectricMeterDataSetting()
        {
			List<DataUnitInfo> dataInfoList = new List<DataUnitInfo>();

			DataUnitInfo di;
			di = new DataUnitInfo(); di.Name = "通信地址";			di.Offset = 0;		di.Length = 1; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "通信波特率"; di.Offset = 1; di.Length = 1; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "电流方向"; di.Offset = 10; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "工作时间(选项)"; di.Offset = 12; di.Length = 4; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "CT变化(选项)"; di.Offset = 16; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相电压"; di.Offset = 100; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相电压"; di.Offset = 102; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相电压"; di.Offset = 104; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "AB线电压"; di.Offset = 106; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "BC线电压"; di.Offset = 108; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "CA线电压"; di.Offset = 110; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相电流"; di.Offset = 112; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相电流"; di.Offset = 114; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相电流"; di.Offset = 116; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相有功功率"; di.Offset = 118; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相有功功率"; di.Offset = 120; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相有功功率"; di.Offset = 122; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "总有功功率"; di.Offset = 124; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相无功功率"; di.Offset = 126; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相无功功率"; di.Offset = 128; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相无功功率"; di.Offset = 130; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "总无功功率"; di.Offset = 132; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相视在功率"; di.Offset = 134; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相视在功率"; di.Offset = 136; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相视在功率"; di.Offset = 138; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "总视在功率"; di.Offset = 140; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "A相功率因数"; di.Offset = 142; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "B相功率因数"; di.Offset = 144; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "C相功率因数"; di.Offset = 146; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "总功率因数"; di.Offset = 148; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "频率"; di.Offset = 150; di.Length = 2; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "正向(吸收)有功电能"; di.Offset = 152; di.Length = 4; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "反向(释放)有功电能"; di.Offset = 156; di.Length = 4; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "感性无功电能"; di.Offset = 160; di.Length = 4; dataInfoList.Add(di);
			di = new DataUnitInfo(); di.Name = "容性无功电能"; di.Offset = 164; di.Length = 4; dataInfoList.Add(di);

			return dataInfoList;
        }
    }

    public class DataUnitInfo
    {
        string _name = "";
        // 名称
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        int _offset = 0;
        // 起始偏移量
        public int Offset
        {
            get { return _offset; }
            set { _offset = value; }
        }

        int _length = 2;
        // 长度
        public int Length
        {
            get { return _length; }
            set { _length = value; }
        }

		ulong _value = 0;

		public ulong Value
		{
			get { return _value; }
			set { _value = value; }
		}
    }
}
