namespace _13
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.del_button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.resultTB = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.valueTB = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.calc_button = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.addList_button = new System.Windows.Forms.Button();  // Новая кнопка
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.selectedTB = new System.Windows.Forms.TextBox();
            this.toAddTextBox = new System.Windows.Forms.TextBox();
            this.add_button = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // del_button
            // 
            this.del_button.Location = new System.Drawing.Point(156, 70);
            this.del_button.Name = "del_button";
            this.del_button.Size = new System.Drawing.Size(75, 23);
            this.del_button.TabIndex = 1;
            this.del_button.Text = "Delete";
            this.del_button.UseVisualStyleBackColor = true;
            this.del_button.Click += new System.EventHandler(this.del_button_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(31, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 16);
            this.label1.TabIndex = 4;
            this.label1.Text = "Value";
            // 
            // resultTB
            // 
            this.resultTB.Location = new System.Drawing.Point(17, 100);
            this.resultTB.Name = "resultTB";
            this.resultTB.Size = new System.Drawing.Size(100, 22);
            this.resultTB.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(31, 81);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 16);
            this.label2.TabIndex = 5;
            this.label2.Text = "Result";
            // 
            // valueTB
            // 
            this.valueTB.Location = new System.Drawing.Point(17, 56);
            this.valueTB.Name = "valueTB";
            this.valueTB.Size = new System.Drawing.Size(100, 22);
            this.valueTB.TabIndex = 2;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.calc_button);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.resultTB);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.valueTB);
            this.groupBox1.Location = new System.Drawing.Point(109, 247);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(220, 146);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Calculate";
            // 
            // calc_button
            // 
            this.calc_button.Location = new System.Drawing.Point(139, 55);
            this.calc_button.Name = "calc_button";
            this.calc_button.Size = new System.Drawing.Size(75, 23);
            this.calc_button.TabIndex = 6;
            this.calc_button.Text = "Enter";
            this.calc_button.UseVisualStyleBackColor = true;
            this.calc_button.Click += new System.EventHandler(this.calc_button_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.addList_button);  // Размещение новой кнопки
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.selectedTB);
            this.groupBox2.Controls.Add(this.toAddTextBox);
            this.groupBox2.Controls.Add(this.add_button);
            this.groupBox2.Controls.Add(this.listBox1);
            this.groupBox2.Controls.Add(this.del_button);
            this.groupBox2.Location = new System.Drawing.Point(454, 58);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(237, 286);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "List";
            // 
            // addList_button
            // 
            this.addList_button.Location = new System.Drawing.Point(156, 193);  // Позиция новой кнопки
            this.addList_button.Name = "addList_button";
            this.addList_button.Size = new System.Drawing.Size(75, 23);
            this.addList_button.TabIndex = 12;
            this.addList_button.Text = "Add List";
            this.addList_button.UseVisualStyleBackColor = true;
            this.addList_button.Click += new System.EventHandler(this.addList_button_Click);  // Обработчик события
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(141, 221);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 16);
            this.label4.TabIndex = 11;
            this.label4.Text = "string to add";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 221);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(94, 16);
            this.label3.TabIndex = 10;
            this.label3.Text = "selected string";
            // 
            // selectedTB
            // 
            this.selectedTB.Location = new System.Drawing.Point(18, 245);
            this.selectedTB.Name = "selectedTB";
            this.selectedTB.Size = new System.Drawing.Size(100, 22);
            this.selectedTB.TabIndex = 9;
            // 
            // toAddTextBox
            // 
            this.toAddTextBox.Location = new System.Drawing.Point(137, 245);
            this.toAddTextBox.Name = "toAddTextBox";
            this.toAddTextBox.Size = new System.Drawing.Size(100, 22);
            this.toAddTextBox.TabIndex = 8;
            // 
            // add_button
            // 
            this.add_button.Location = new System.Drawing.Point(156, 164);
            this.add_button.Name = "add_button";
            this.add_button.Size = new System.Drawing.Size(75, 23);
            this.add_button.TabIndex = 7;
            this.add_button.Text = "Add";
            this.add_button.UseVisualStyleBackColor = true;
            this.add_button.Click += new System.EventHandler(this.add_button_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 16;
            this.listBox1.Location = new System.Drawing.Point(18, 21);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(128, 196);
            this.listBox1.TabIndex = 0;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.Button del_button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox resultTB;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox valueTB;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button calc_button;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button addList_button;  // Добавлено в код
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox selectedTB;
        private System.Windows.Forms.TextBox toAddTextBox;
        private System.Windows.Forms.Button add_button;
        private System.Windows.Forms.ListBox listBox1;
    }
}
