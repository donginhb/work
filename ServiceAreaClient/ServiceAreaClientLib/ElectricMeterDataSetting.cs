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
    class ElectricMeterDataSetting
    {
        // 读数信息列表
        public ElectricMeterDataInfo[] dataArray = new ElectricMeterDataInfo[40];

		/// <summary>
		/// 电表读数设定类, 注意高字节在前
		/// </summary>
        public ElectricMeterDataSetting()
        {
			for (int i = 0; i < dataArray.Length; i++)
			{
				dataArray[i] = new ElectricMeterDataInfo();
			}
            dataArray[0].Name = "通信地址";				dataArray[0].Offset = 0;	dataArray[0].Length = 1;
            dataArray[1].Name = "通信波特率";			dataArray[1].Offset = 1;	dataArray[1].Length = 1;
            dataArray[2].Name = "电流方向";				dataArray[2].Offset = 10;	dataArray[2].Length = 2;
            dataArray[3].Name = "工作时间(选项)";		dataArray[3].Offset = 12;	dataArray[3].Length = 4;
            dataArray[4].Name = "CT变化(选项)";			dataArray[4].Offset = 16;	dataArray[4].Length = 2;
            dataArray[5].Name = "A相电压";				dataArray[5].Offset = 100;	dataArray[5].Length = 2;
            dataArray[6].Name = "B相电压";				dataArray[6].Offset = 102;	dataArray[6].Length = 2;
            dataArray[7].Name = "C相电压";				dataArray[7].Offset = 104;	dataArray[7].Length = 2;
            dataArray[8].Name = "AB线电压";				dataArray[8].Offset = 106;	dataArray[8].Length = 2;
            dataArray[9].Name = "BC线电压";				dataArray[9].Offset = 108;	dataArray[9].Length = 2;
            dataArray[10].Name = "CA线电压";			dataArray[10].Offset = 110; dataArray[10].Length = 2;
            dataArray[11].Name = "A相电流";				dataArray[11].Offset = 112; dataArray[11].Length = 2;
            dataArray[12].Name = "B相电流";				dataArray[12].Offset = 114; dataArray[12].Length = 2;
            dataArray[13].Name = "C相电流";				dataArray[13].Offset = 116; dataArray[13].Length = 2;
            dataArray[14].Name = "A相有功功率";			dataArray[14].Offset = 118; dataArray[14].Length = 2;
            dataArray[15].Name = "B相有功功率";			dataArray[15].Offset = 120; dataArray[15].Length = 2;
            dataArray[16].Name = "C相有功功率";			dataArray[16].Offset = 122; dataArray[16].Length = 2;
            dataArray[17].Name = "总有功功率";			dataArray[17].Offset = 124; dataArray[17].Length = 2;
            dataArray[18].Name = "A相无功功率";			dataArray[18].Offset = 126; dataArray[18].Length = 2;
            dataArray[19].Name = "B相无功功率";			dataArray[19].Offset = 128; dataArray[19].Length = 2;
            dataArray[20].Name = "C相无功功率";			dataArray[20].Offset = 130; dataArray[20].Length = 2;
            dataArray[21].Name = "总无功功率";			dataArray[21].Offset = 132; dataArray[21].Length = 2;
            dataArray[22].Name = "A相视在功率";			dataArray[22].Offset = 134; dataArray[22].Length = 2;
            dataArray[23].Name = "B相视在功率";			dataArray[23].Offset = 136; dataArray[23].Length = 2;
            dataArray[24].Name = "C相视在功率";			dataArray[24].Offset = 138; dataArray[24].Length = 2;
            dataArray[25].Name = "总视在功率";			dataArray[25].Offset = 140; dataArray[25].Length = 2;
            dataArray[26].Name = "A相功率因数";			dataArray[26].Offset = 142; dataArray[26].Length = 2;
            dataArray[27].Name = "B相功率因数";			dataArray[27].Offset = 144; dataArray[27].Length = 2;
            dataArray[28].Name = "C相功率因数";			dataArray[28].Offset = 146; dataArray[28].Length = 2;
            dataArray[29].Name = "总功率因数";			dataArray[29].Offset = 148; dataArray[29].Length = 2;
            dataArray[30].Name = "频率";				dataArray[30].Offset = 150; dataArray[30].Length = 2;
            dataArray[31].Name = "正向(吸收)有功电能";	dataArray[31].Offset = 152; dataArray[31].Length = 4;
            dataArray[32].Name = "反向(释放)有功电能";	dataArray[32].Offset = 156; dataArray[32].Length = 4;
            dataArray[33].Name = "感性无功电能";		dataArray[33].Offset = 160; dataArray[33].Length = 4;
            dataArray[34].Name = "容性无功电能";		dataArray[34].Offset = 164; dataArray[34].Length = 4;
        }
    }

    class ElectricMeterDataInfo
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
