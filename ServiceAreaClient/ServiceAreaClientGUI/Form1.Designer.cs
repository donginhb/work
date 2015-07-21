namespace UIManager
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			this.btnStart = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.cbxPassword = new System.Windows.Forms.CheckBox();
			this.tbxPassword = new System.Windows.Forms.TextBox();
			this.tbxUsrName = new System.Windows.Forms.TextBox();
			this.label14 = new System.Windows.Forms.Label();
			this.tbxDBName = new System.Windows.Forms.TextBox();
			this.label13 = new System.Windows.Forms.Label();
			this.tbxPortNum2 = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.tbxIP5 = new System.Windows.Forms.TextBox();
			this.label8 = new System.Windows.Forms.Label();
			this.tbxIP6 = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.tbxIP7 = new System.Windows.Forms.TextBox();
			this.label10 = new System.Windows.Forms.Label();
			this.tbxIP8 = new System.Windows.Forms.TextBox();
			this.label11 = new System.Windows.Forms.Label();
			this.label12 = new System.Windows.Forms.Label();
			this.btnTest = new System.Windows.Forms.Button();
			this.listView1 = new System.Windows.Forms.ListView();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnEdit = new System.Windows.Forms.Button();
			this.btnDel = new System.Windows.Forms.Button();
			this.tbxUpdatePeriod = new System.Windows.Forms.TextBox();
			this.btnAdd = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnStart
			// 
			this.btnStart.Location = new System.Drawing.Point(394, 476);
			this.btnStart.Name = "btnStart";
			this.btnStart.Size = new System.Drawing.Size(101, 54);
			this.btnStart.TabIndex = 0;
			this.btnStart.Text = "Start";
			this.btnStart.UseVisualStyleBackColor = true;
			this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(12, 12);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox1.Size = new System.Drawing.Size(361, 518);
			this.textBox1.TabIndex = 1;
			// 
			// timer1
			// 
			this.timer1.Interval = 1000;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.cbxPassword);
			this.groupBox2.Controls.Add(this.tbxPassword);
			this.groupBox2.Controls.Add(this.tbxUsrName);
			this.groupBox2.Controls.Add(this.label14);
			this.groupBox2.Controls.Add(this.tbxDBName);
			this.groupBox2.Controls.Add(this.label13);
			this.groupBox2.Controls.Add(this.tbxPortNum2);
			this.groupBox2.Controls.Add(this.label7);
			this.groupBox2.Controls.Add(this.tbxIP5);
			this.groupBox2.Controls.Add(this.label8);
			this.groupBox2.Controls.Add(this.tbxIP6);
			this.groupBox2.Controls.Add(this.label9);
			this.groupBox2.Controls.Add(this.tbxIP7);
			this.groupBox2.Controls.Add(this.label10);
			this.groupBox2.Controls.Add(this.tbxIP8);
			this.groupBox2.Controls.Add(this.label11);
			this.groupBox2.Controls.Add(this.label12);
			this.groupBox2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.groupBox2.Location = new System.Drawing.Point(394, 12);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(498, 132);
			this.groupBox2.TabIndex = 4;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "服务器端";
			// 
			// cbxPassword
			// 
			this.cbxPassword.AutoSize = true;
			this.cbxPassword.Location = new System.Drawing.Point(322, 69);
			this.cbxPassword.Name = "cbxPassword";
			this.cbxPassword.Size = new System.Drawing.Size(72, 16);
			this.cbxPassword.TabIndex = 4;
			this.cbxPassword.Text = "记住密码";
			this.cbxPassword.UseVisualStyleBackColor = true;
			// 
			// tbxPassword
			// 
			this.tbxPassword.Location = new System.Drawing.Point(276, 88);
			this.tbxPassword.Name = "tbxPassword";
			this.tbxPassword.PasswordChar = '*';
			this.tbxPassword.Size = new System.Drawing.Size(118, 21);
			this.tbxPassword.TabIndex = 2;
			// 
			// tbxUsrName
			// 
			this.tbxUsrName.Location = new System.Drawing.Point(147, 88);
			this.tbxUsrName.Name = "tbxUsrName";
			this.tbxUsrName.Size = new System.Drawing.Size(98, 21);
			this.tbxUsrName.TabIndex = 2;
			// 
			// label14
			// 
			this.label14.AutoSize = true;
			this.label14.Location = new System.Drawing.Point(274, 70);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(35, 12);
			this.label14.TabIndex = 3;
			this.label14.Text = "密码:";
			// 
			// tbxDBName
			// 
			this.tbxDBName.Location = new System.Drawing.Point(6, 88);
			this.tbxDBName.Name = "tbxDBName";
			this.tbxDBName.Size = new System.Drawing.Size(112, 21);
			this.tbxDBName.TabIndex = 2;
			// 
			// label13
			// 
			this.label13.AutoSize = true;
			this.label13.Location = new System.Drawing.Point(145, 70);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(47, 12);
			this.label13.TabIndex = 3;
			this.label13.Text = "用户名:";
			// 
			// tbxPortNum2
			// 
			this.tbxPortNum2.Location = new System.Drawing.Point(205, 39);
			this.tbxPortNum2.Name = "tbxPortNum2";
			this.tbxPortNum2.Size = new System.Drawing.Size(73, 21);
			this.tbxPortNum2.TabIndex = 2;
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(4, 70);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(59, 12);
			this.label7.TabIndex = 3;
			this.label7.Text = "数据库名:";
			// 
			// tbxIP5
			// 
			this.tbxIP5.Location = new System.Drawing.Point(6, 39);
			this.tbxIP5.Name = "tbxIP5";
			this.tbxIP5.Size = new System.Drawing.Size(28, 21);
			this.tbxIP5.TabIndex = 2;
			this.tbxIP5.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(203, 22);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(47, 12);
			this.label8.TabIndex = 3;
			this.label8.Text = "端口号:";
			// 
			// tbxIP6
			// 
			this.tbxIP6.Location = new System.Drawing.Point(48, 39);
			this.tbxIP6.Name = "tbxIP6";
			this.tbxIP6.Size = new System.Drawing.Size(28, 21);
			this.tbxIP6.TabIndex = 2;
			this.tbxIP6.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(121, 40);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(11, 12);
			this.label9.TabIndex = 3;
			this.label9.Text = ".";
			// 
			// tbxIP7
			// 
			this.tbxIP7.Location = new System.Drawing.Point(90, 39);
			this.tbxIP7.Name = "tbxIP7";
			this.tbxIP7.Size = new System.Drawing.Size(28, 21);
			this.tbxIP7.TabIndex = 2;
			this.tbxIP7.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(78, 42);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(11, 12);
			this.label10.TabIndex = 3;
			this.label10.Text = ".";
			// 
			// tbxIP8
			// 
			this.tbxIP8.Location = new System.Drawing.Point(132, 39);
			this.tbxIP8.Name = "tbxIP8";
			this.tbxIP8.Size = new System.Drawing.Size(28, 21);
			this.tbxIP8.TabIndex = 2;
			this.tbxIP8.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Location = new System.Drawing.Point(36, 42);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(11, 12);
			this.label11.TabIndex = 3;
			this.label11.Text = ".";
			// 
			// label12
			// 
			this.label12.AutoSize = true;
			this.label12.Location = new System.Drawing.Point(4, 22);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(47, 12);
			this.label12.TabIndex = 3;
			this.label12.Text = "IP地址:";
			// 
			// btnTest
			// 
			this.btnTest.Location = new System.Drawing.Point(779, 476);
			this.btnTest.Name = "btnTest";
			this.btnTest.Size = new System.Drawing.Size(101, 54);
			this.btnTest.TabIndex = 5;
			this.btnTest.Text = "DBTest";
			this.btnTest.UseVisualStyleBackColor = true;
			this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
			// 
			// listView1
			// 
			this.listView1.CheckBoxes = true;
			this.listView1.FullRowSelect = true;
			this.listView1.Location = new System.Drawing.Point(6, 18);
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(409, 234);
			this.listView1.TabIndex = 6;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.View = System.Windows.Forms.View.Details;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnEdit);
			this.groupBox1.Controls.Add(this.btnDel);
			this.groupBox1.Controls.Add(this.tbxUpdatePeriod);
			this.groupBox1.Controls.Add(this.btnAdd);
			this.groupBox1.Controls.Add(this.listView1);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(394, 163);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(498, 307);
			this.groupBox1.TabIndex = 7;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "设备列表";
			// 
			// btnEdit
			// 
			this.btnEdit.Location = new System.Drawing.Point(423, 146);
			this.btnEdit.Name = "btnEdit";
			this.btnEdit.Size = new System.Drawing.Size(65, 37);
			this.btnEdit.TabIndex = 7;
			this.btnEdit.Text = "编辑";
			this.btnEdit.UseVisualStyleBackColor = true;
			this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
			// 
			// btnDel
			// 
			this.btnDel.Location = new System.Drawing.Point(423, 81);
			this.btnDel.Name = "btnDel";
			this.btnDel.Size = new System.Drawing.Size(65, 37);
			this.btnDel.TabIndex = 7;
			this.btnDel.Text = "删除";
			this.btnDel.UseVisualStyleBackColor = true;
			this.btnDel.Click += new System.EventHandler(this.btnDel_Click);
			// 
			// tbxUpdatePeriod
			// 
			this.tbxUpdatePeriod.Location = new System.Drawing.Point(65, 272);
			this.tbxUpdatePeriod.Name = "tbxUpdatePeriod";
			this.tbxUpdatePeriod.Size = new System.Drawing.Size(67, 19);
			this.tbxUpdatePeriod.TabIndex = 2;
			this.tbxUpdatePeriod.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// btnAdd
			// 
			this.btnAdd.Location = new System.Drawing.Point(423, 18);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(65, 37);
			this.btnAdd.TabIndex = 7;
			this.btnAdd.Text = "增加";
			this.btnAdd.UseVisualStyleBackColor = true;
			this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(138, 275);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(17, 12);
			this.label2.TabIndex = 3;
			this.label2.Text = "分";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(4, 275);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(55, 12);
			this.label1.TabIndex = 3;
			this.label1.Text = "更新周期:";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(904, 546);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.btnTest);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.btnStart);
			this.MaximizeBox = false;
			this.Name = "Form1";
			this.Text = "ServiceAreaClient";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbxPortNum2;
        private System.Windows.Forms.TextBox tbxIP5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbxIP6;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbxIP7;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox tbxIP8;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.TextBox tbxPassword;
        private System.Windows.Forms.TextBox tbxUsrName;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox tbxDBName;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox cbxPassword;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button btnDel;
		private System.Windows.Forms.Button btnAdd;
		private System.Windows.Forms.TextBox tbxUpdatePeriod;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btnEdit;
    }
}

