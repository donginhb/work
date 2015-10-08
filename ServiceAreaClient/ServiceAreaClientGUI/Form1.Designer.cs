namespace ServiceAreaClient
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
			this.btnStart = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.listView1 = new System.Windows.Forms.ListView();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnEdit1 = new System.Windows.Forms.Button();
			this.btnDel1 = new System.Windows.Forms.Button();
			this.btnAdd1 = new System.Windows.Forms.Button();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.btnEdit2 = new System.Windows.Forms.Button();
			this.btnDel2 = new System.Windows.Forms.Button();
			this.btnAdd2 = new System.Windows.Forms.Button();
			this.listView2 = new System.Windows.Forms.ListView();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.btnEdit3 = new System.Windows.Forms.Button();
			this.btnDel3 = new System.Windows.Forms.Button();
			this.btnAdd3 = new System.Windows.Forms.Button();
			this.listView3 = new System.Windows.Forms.ListView();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.textBox3 = new System.Windows.Forms.TextBox();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.btnEdit4 = new System.Windows.Forms.Button();
			this.btnDel4 = new System.Windows.Forms.Button();
			this.btnAdd4 = new System.Windows.Forms.Button();
			this.listView4 = new System.Windows.Forms.ListView();
			this.textBox4 = new System.Windows.Forms.TextBox();
			this.textBox5 = new System.Windows.Forms.TextBox();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.btnEdit5 = new System.Windows.Forms.Button();
			this.btnDel5 = new System.Windows.Forms.Button();
			this.btnAdd5 = new System.Windows.Forms.Button();
			this.listView5 = new System.Windows.Forms.ListView();
			this.btnSetting = new System.Windows.Forms.Button();
			this.cbxAutoStart = new System.Windows.Forms.CheckBox();
			this.groupBox1.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.groupBox6.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnStart
			// 
			this.btnStart.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.btnStart.Location = new System.Drawing.Point(934, 35);
			this.btnStart.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnStart.Name = "btnStart";
			this.btnStart.Size = new System.Drawing.Size(100, 62);
			this.btnStart.TabIndex = 0;
			this.btnStart.Text = "Start";
			this.btnStart.UseVisualStyleBackColor = true;
			this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(12, 10);
			this.textBox1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox1.Size = new System.Drawing.Size(305, 171);
			this.textBox1.TabIndex = 1;
			// 
			// listView1
			// 
			this.listView1.CheckBoxes = true;
			this.listView1.FullRowSelect = true;
			this.listView1.GridLines = true;
			this.listView1.Location = new System.Drawing.Point(7, 25);
			this.listView1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.listView1.MultiSelect = false;
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(476, 127);
			this.listView1.TabIndex = 6;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.View = System.Windows.Forms.View.Details;
			this.listView1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView1_MouseDoubleClick);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnEdit1);
			this.groupBox1.Controls.Add(this.btnDel1);
			this.groupBox1.Controls.Add(this.btnAdd1);
			this.groupBox1.Controls.Add(this.listView1);
			this.groupBox1.Location = new System.Drawing.Point(330, 10);
			this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox1.Size = new System.Drawing.Size(581, 171);
			this.groupBox1.TabIndex = 7;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "电表";
			// 
			// btnEdit1
			// 
			this.btnEdit1.Location = new System.Drawing.Point(493, 115);
			this.btnEdit1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnEdit1.Name = "btnEdit1";
			this.btnEdit1.Size = new System.Drawing.Size(76, 37);
			this.btnEdit1.TabIndex = 7;
			this.btnEdit1.Text = "编辑";
			this.btnEdit1.UseVisualStyleBackColor = true;
			this.btnEdit1.Click += new System.EventHandler(this.btnEdit1_Click);
			// 
			// btnDel1
			// 
			this.btnDel1.Location = new System.Drawing.Point(493, 70);
			this.btnDel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnDel1.Name = "btnDel1";
			this.btnDel1.Size = new System.Drawing.Size(76, 37);
			this.btnDel1.TabIndex = 7;
			this.btnDel1.Text = "删除";
			this.btnDel1.UseVisualStyleBackColor = true;
			this.btnDel1.Click += new System.EventHandler(this.btnDel1_Click);
			// 
			// btnAdd1
			// 
			this.btnAdd1.Location = new System.Drawing.Point(493, 25);
			this.btnAdd1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnAdd1.Name = "btnAdd1";
			this.btnAdd1.Size = new System.Drawing.Size(76, 37);
			this.btnAdd1.TabIndex = 7;
			this.btnAdd1.Text = "增加";
			this.btnAdd1.UseVisualStyleBackColor = true;
			this.btnAdd1.Click += new System.EventHandler(this.btnAdd1_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.btnEdit2);
			this.groupBox3.Controls.Add(this.btnDel2);
			this.groupBox3.Controls.Add(this.btnAdd2);
			this.groupBox3.Controls.Add(this.listView2);
			this.groupBox3.Location = new System.Drawing.Point(330, 185);
			this.groupBox3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox3.Size = new System.Drawing.Size(581, 172);
			this.groupBox3.TabIndex = 7;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "人流计数";
			// 
			// btnEdit2
			// 
			this.btnEdit2.Location = new System.Drawing.Point(493, 116);
			this.btnEdit2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnEdit2.Name = "btnEdit2";
			this.btnEdit2.Size = new System.Drawing.Size(76, 37);
			this.btnEdit2.TabIndex = 7;
			this.btnEdit2.Text = "编辑";
			this.btnEdit2.UseVisualStyleBackColor = true;
			this.btnEdit2.Click += new System.EventHandler(this.btnEdit2_Click);
			// 
			// btnDel2
			// 
			this.btnDel2.Location = new System.Drawing.Point(493, 71);
			this.btnDel2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnDel2.Name = "btnDel2";
			this.btnDel2.Size = new System.Drawing.Size(76, 37);
			this.btnDel2.TabIndex = 7;
			this.btnDel2.Text = "删除";
			this.btnDel2.UseVisualStyleBackColor = true;
			this.btnDel2.Click += new System.EventHandler(this.btnDel2_Click);
			// 
			// btnAdd2
			// 
			this.btnAdd2.Location = new System.Drawing.Point(493, 25);
			this.btnAdd2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnAdd2.Name = "btnAdd2";
			this.btnAdd2.Size = new System.Drawing.Size(76, 37);
			this.btnAdd2.TabIndex = 7;
			this.btnAdd2.Text = "增加";
			this.btnAdd2.UseVisualStyleBackColor = true;
			this.btnAdd2.Click += new System.EventHandler(this.btnAdd2_Click);
			// 
			// listView2
			// 
			this.listView2.CheckBoxes = true;
			this.listView2.FullRowSelect = true;
			this.listView2.GridLines = true;
			this.listView2.Location = new System.Drawing.Point(7, 25);
			this.listView2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.listView2.MultiSelect = false;
			this.listView2.Name = "listView2";
			this.listView2.Size = new System.Drawing.Size(476, 128);
			this.listView2.TabIndex = 6;
			this.listView2.UseCompatibleStateImageBehavior = false;
			this.listView2.View = System.Windows.Forms.View.Details;
			this.listView2.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView2_MouseDoubleClick);
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.btnEdit3);
			this.groupBox4.Controls.Add(this.btnDel3);
			this.groupBox4.Controls.Add(this.btnAdd3);
			this.groupBox4.Controls.Add(this.listView3);
			this.groupBox4.Location = new System.Drawing.Point(330, 358);
			this.groupBox4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox4.Size = new System.Drawing.Size(581, 172);
			this.groupBox4.TabIndex = 7;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "室温";
			// 
			// btnEdit3
			// 
			this.btnEdit3.Location = new System.Drawing.Point(493, 116);
			this.btnEdit3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnEdit3.Name = "btnEdit3";
			this.btnEdit3.Size = new System.Drawing.Size(76, 37);
			this.btnEdit3.TabIndex = 7;
			this.btnEdit3.Text = "编辑";
			this.btnEdit3.UseVisualStyleBackColor = true;
			this.btnEdit3.Click += new System.EventHandler(this.btnEdit3_Click);
			// 
			// btnDel3
			// 
			this.btnDel3.Location = new System.Drawing.Point(493, 71);
			this.btnDel3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnDel3.Name = "btnDel3";
			this.btnDel3.Size = new System.Drawing.Size(76, 37);
			this.btnDel3.TabIndex = 7;
			this.btnDel3.Text = "删除";
			this.btnDel3.UseVisualStyleBackColor = true;
			this.btnDel3.Click += new System.EventHandler(this.btnDel3_Click);
			// 
			// btnAdd3
			// 
			this.btnAdd3.Location = new System.Drawing.Point(493, 25);
			this.btnAdd3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnAdd3.Name = "btnAdd3";
			this.btnAdd3.Size = new System.Drawing.Size(76, 37);
			this.btnAdd3.TabIndex = 7;
			this.btnAdd3.Text = "增加";
			this.btnAdd3.UseVisualStyleBackColor = true;
			this.btnAdd3.Click += new System.EventHandler(this.btnAdd3_Click);
			// 
			// listView3
			// 
			this.listView3.CheckBoxes = true;
			this.listView3.FullRowSelect = true;
			this.listView3.GridLines = true;
			this.listView3.Location = new System.Drawing.Point(7, 25);
			this.listView3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.listView3.MultiSelect = false;
			this.listView3.Name = "listView3";
			this.listView3.Size = new System.Drawing.Size(476, 128);
			this.listView3.TabIndex = 6;
			this.listView3.UseCompatibleStateImageBehavior = false;
			this.listView3.View = System.Windows.Forms.View.Details;
			this.listView3.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView3_MouseDoubleClick);
			// 
			// textBox2
			// 
			this.textBox2.Location = new System.Drawing.Point(12, 189);
			this.textBox2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.textBox2.Multiline = true;
			this.textBox2.Name = "textBox2";
			this.textBox2.ReadOnly = true;
			this.textBox2.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox2.Size = new System.Drawing.Size(305, 168);
			this.textBox2.TabIndex = 1;
			// 
			// textBox3
			// 
			this.textBox3.Location = new System.Drawing.Point(12, 365);
			this.textBox3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.textBox3.Multiline = true;
			this.textBox3.Name = "textBox3";
			this.textBox3.ReadOnly = true;
			this.textBox3.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox3.Size = new System.Drawing.Size(305, 165);
			this.textBox3.TabIndex = 1;
			// 
			// groupBox5
			// 
			this.groupBox5.Controls.Add(this.btnEdit4);
			this.groupBox5.Controls.Add(this.btnDel4);
			this.groupBox5.Controls.Add(this.btnAdd4);
			this.groupBox5.Controls.Add(this.listView4);
			this.groupBox5.Location = new System.Drawing.Point(330, 532);
			this.groupBox5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox5.Size = new System.Drawing.Size(581, 172);
			this.groupBox5.TabIndex = 7;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "水表";
			// 
			// btnEdit4
			// 
			this.btnEdit4.Location = new System.Drawing.Point(493, 116);
			this.btnEdit4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnEdit4.Name = "btnEdit4";
			this.btnEdit4.Size = new System.Drawing.Size(76, 37);
			this.btnEdit4.TabIndex = 7;
			this.btnEdit4.Text = "编辑";
			this.btnEdit4.UseVisualStyleBackColor = true;
			this.btnEdit4.Click += new System.EventHandler(this.btnEdit4_Click);
			// 
			// btnDel4
			// 
			this.btnDel4.Location = new System.Drawing.Point(493, 71);
			this.btnDel4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnDel4.Name = "btnDel4";
			this.btnDel4.Size = new System.Drawing.Size(76, 37);
			this.btnDel4.TabIndex = 7;
			this.btnDel4.Text = "删除";
			this.btnDel4.UseVisualStyleBackColor = true;
			this.btnDel4.Click += new System.EventHandler(this.btnDel4_Click);
			// 
			// btnAdd4
			// 
			this.btnAdd4.Location = new System.Drawing.Point(493, 25);
			this.btnAdd4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnAdd4.Name = "btnAdd4";
			this.btnAdd4.Size = new System.Drawing.Size(76, 37);
			this.btnAdd4.TabIndex = 7;
			this.btnAdd4.Text = "增加";
			this.btnAdd4.UseVisualStyleBackColor = true;
			this.btnAdd4.Click += new System.EventHandler(this.btnAdd4_Click);
			// 
			// listView4
			// 
			this.listView4.CheckBoxes = true;
			this.listView4.FullRowSelect = true;
			this.listView4.GridLines = true;
			this.listView4.Location = new System.Drawing.Point(7, 25);
			this.listView4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.listView4.MultiSelect = false;
			this.listView4.Name = "listView4";
			this.listView4.Size = new System.Drawing.Size(476, 128);
			this.listView4.TabIndex = 6;
			this.listView4.UseCompatibleStateImageBehavior = false;
			this.listView4.View = System.Windows.Forms.View.Details;
			this.listView4.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView4_MouseDoubleClick);
			// 
			// textBox4
			// 
			this.textBox4.Location = new System.Drawing.Point(14, 539);
			this.textBox4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.textBox4.Multiline = true;
			this.textBox4.Name = "textBox4";
			this.textBox4.ReadOnly = true;
			this.textBox4.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox4.Size = new System.Drawing.Size(305, 165);
			this.textBox4.TabIndex = 1;
			// 
			// textBox5
			// 
			this.textBox5.Location = new System.Drawing.Point(14, 713);
			this.textBox5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.textBox5.Multiline = true;
			this.textBox5.Name = "textBox5";
			this.textBox5.ReadOnly = true;
			this.textBox5.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox5.Size = new System.Drawing.Size(305, 165);
			this.textBox5.TabIndex = 1;
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.btnEdit5);
			this.groupBox6.Controls.Add(this.btnDel5);
			this.groupBox6.Controls.Add(this.btnAdd5);
			this.groupBox6.Controls.Add(this.listView5);
			this.groupBox6.Location = new System.Drawing.Point(330, 706);
			this.groupBox6.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.groupBox6.Size = new System.Drawing.Size(581, 172);
			this.groupBox6.TabIndex = 7;
			this.groupBox6.TabStop = false;
			this.groupBox6.Text = "水温";
			// 
			// btnEdit5
			// 
			this.btnEdit5.Location = new System.Drawing.Point(493, 116);
			this.btnEdit5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnEdit5.Name = "btnEdit5";
			this.btnEdit5.Size = new System.Drawing.Size(76, 37);
			this.btnEdit5.TabIndex = 7;
			this.btnEdit5.Text = "编辑";
			this.btnEdit5.UseVisualStyleBackColor = true;
			this.btnEdit5.Click += new System.EventHandler(this.btnEdit5_Click);
			// 
			// btnDel5
			// 
			this.btnDel5.Location = new System.Drawing.Point(493, 71);
			this.btnDel5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnDel5.Name = "btnDel5";
			this.btnDel5.Size = new System.Drawing.Size(76, 37);
			this.btnDel5.TabIndex = 7;
			this.btnDel5.Text = "删除";
			this.btnDel5.UseVisualStyleBackColor = true;
			this.btnDel5.Click += new System.EventHandler(this.btnDel5_Click);
			// 
			// btnAdd5
			// 
			this.btnAdd5.Location = new System.Drawing.Point(493, 25);
			this.btnAdd5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.btnAdd5.Name = "btnAdd5";
			this.btnAdd5.Size = new System.Drawing.Size(76, 37);
			this.btnAdd5.TabIndex = 7;
			this.btnAdd5.Text = "增加";
			this.btnAdd5.UseVisualStyleBackColor = true;
			this.btnAdd5.Click += new System.EventHandler(this.btnAdd5_Click);
			// 
			// listView5
			// 
			this.listView5.CheckBoxes = true;
			this.listView5.FullRowSelect = true;
			this.listView5.GridLines = true;
			this.listView5.Location = new System.Drawing.Point(7, 25);
			this.listView5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.listView5.MultiSelect = false;
			this.listView5.Name = "listView5";
			this.listView5.Size = new System.Drawing.Size(476, 128);
			this.listView5.TabIndex = 6;
			this.listView5.UseCompatibleStateImageBehavior = false;
			this.listView5.View = System.Windows.Forms.View.Details;
			this.listView5.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView5_MouseDoubleClick);
			// 
			// btnSetting
			// 
			this.btnSetting.Location = new System.Drawing.Point(934, 731);
			this.btnSetting.Name = "btnSetting";
			this.btnSetting.Size = new System.Drawing.Size(100, 31);
			this.btnSetting.TabIndex = 8;
			this.btnSetting.Text = "Setting";
			this.btnSetting.UseVisualStyleBackColor = true;
			this.btnSetting.Click += new System.EventHandler(this.btnSetting_Click);
			// 
			// cbxAutoStart
			// 
			this.cbxAutoStart.AutoSize = true;
			this.cbxAutoStart.Location = new System.Drawing.Point(935, 831);
			this.cbxAutoStart.Name = "cbxAutoStart";
			this.cbxAutoStart.Size = new System.Drawing.Size(99, 21);
			this.cbxAutoStart.TabIndex = 9;
			this.cbxAutoStart.Text = "下次自动开始";
			this.cbxAutoStart.UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1047, 893);
			this.Controls.Add(this.cbxAutoStart);
			this.Controls.Add(this.btnSetting);
			this.Controls.Add(this.groupBox6);
			this.Controls.Add(this.groupBox5);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.textBox5);
			this.Controls.Add(this.textBox4);
			this.Controls.Add(this.textBox3);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.btnStart);
			this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "ServiceAreaClient_151008";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.groupBox6.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button btnDel1;
		private System.Windows.Forms.Button btnAdd1;
		private System.Windows.Forms.Button btnEdit1;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Button btnEdit2;
		private System.Windows.Forms.Button btnDel2;
		private System.Windows.Forms.Button btnAdd2;
		private System.Windows.Forms.ListView listView2;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Button btnEdit3;
		private System.Windows.Forms.Button btnDel3;
		private System.Windows.Forms.Button btnAdd3;
		private System.Windows.Forms.ListView listView3;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.TextBox textBox3;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.Button btnEdit4;
		private System.Windows.Forms.Button btnDel4;
		private System.Windows.Forms.Button btnAdd4;
		private System.Windows.Forms.ListView listView4;
		private System.Windows.Forms.TextBox textBox4;
		private System.Windows.Forms.TextBox textBox5;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.Button btnEdit5;
		private System.Windows.Forms.Button btnDel5;
		private System.Windows.Forms.Button btnAdd5;
		private System.Windows.Forms.ListView listView5;
		private System.Windows.Forms.Button btnSetting;
		private System.Windows.Forms.CheckBox cbxAutoStart;
    }
}

