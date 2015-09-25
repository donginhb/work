using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;

using VisioForge.Controls;
using VisioForge.Controls.WinForms;
using VisioForge.Types;

namespace VFVideoCapture
{
    [Guid("A2F6B162-50F1-4c78-B4F1-7D4CBFC112F5")]
    [InterfaceType(ComInterfaceType.InterfaceIsDual)]
    public interface IVFVideoCapture
    {
        int Capture_Open();
        int Capture_Close();
        int Capture_DeviceTableUpdate();
        int Capture_DeviceTableGetCount(int eType);
        int Capture_DeviceTableGetName(int eType, int iDeviceIndex, ulong ulpBstrName, int iLimit);
        int Capture_DeviceTableSearchName(int eType, ulong ulpBstrName, int iSize);
        int Capture_DeviceFormatTableGetCount(int eType, int iDeviceIndex);
        int Capture_DeviceFormatTableGetFormat(int eType, int iDeviceIndex, int iFormatIndex,
                                               ulong ulpBstrFormat, int iLimit);
        int Capture_VideoFrameRateTableGetCount(int iVideoIndex);
        int Capture_VideoFrameRateTableGetFrameRate(int iVideoIndex, int iFrameRateIndex,
                                                    ulong ulpBstrFrameRate, int iLimit);
        int Capture_DeviceOpen(int eSession, int iVideoIndex, int iAudioIndex);
        int Capture_IPCameraOpen(ulong ulpBstrURL, int iURLSize,
                                 ulong ulpBstrLogin, int iLoginSize,
                                 ulong ulpBstrPassword, int iPasswordSize);
        int Capture_DeviceClose();
        int Capture_DisplayOpen(int xx, int yy, int lWidth, int lHeight);
        int Capture_DisplayClose();
        int Capture_PreviewPrepare();
        int Capture_CapturePrepare(ulong ulpBstrFile, int iFileSize,
                                   ulong ulpBstrProfile, int iProfileSize,
                                   int iAudioTimeshift, int iSyncView);
        int Capture_DoStart();
        int Capture_DoStop();
        int Capture_DoPause();
        int Capture_DoResume();
        int Capture_GetCount();
        int Capture_GetRecordVolumeLevel();
    }

    [ClassInterface(ClassInterfaceType.None)]
    [Guid("A23FDF4E-A2E4-41b1-94DE-BE1D071FF298")]
    [ComVisible(true)]
    public partial class VFVideoCaptureClass : Form, IVFVideoCapture
    {
        private static VFVideoCaptureClass videoCaptureForm = null;
        private VisioForge.Controls.WinForms.VideoCapture VideoCapture1 = null;
        private CaptureProfile gCurrentProfile;
        private bool gDebug = true;

        private enum CAPTUREDEVICETYPE : int
        {
            CAPTUREDEVICETYPE_AUDIO = 0,
            CAPTUREDEVICETYPE_VIDEO = 1
        }

        private enum CAPTURESESSION : int
        {
            CAPTURESESSION_CAM    = 0,
            CAPTURESESSION_AUDIO  = 1,
            CAPTURESESSION_SCREEN = 2,
            CAPTURESESSION_IPCAM  = 3,
            CAPTURESESSION_NONE   = 4
        }

        private enum CAPTURESTATE
        {
            CAPTURESTATE_Stoped  = 0,
            CAPTURESTATE_Running = 1,
            CAPTURESTATE_Pausing = 2
        }

        private struct CaptureProfile
        {
            public string videoBitRate;
            public string videoFrameRate;
            public string keyFramePerSecond;
            public string audioBitRate;
            public string audioFormat;
            public VFH264Profile videoProfile;
            public VFH264Level videoLevel;
            public string videoFormat;
            public VFAACObject audioProfile;
            public bool grabMouseCursor;
            public int  audioTimeshift;

            public string ipCameraURL;
            public string ipCameraLogin;
            public string ipCameraPassword;

            public string audioDevice;
            public string videoDevice;
            public string outputFilename;
            public VFVideoCaptureMode videoCaptureMode;
            public CAPTURESESSION captureSession;
            public CAPTURESTATE captureState;
            public bool displayShow;
        }

        public int Capture_Open()
        {
            if (videoCaptureForm != null)
            {
                videoCaptureForm.Hide();
                videoCaptureForm.StopCapture();
            }
            else
            {
                videoCaptureForm = new VFVideoCaptureClass();
                videoCaptureForm.InitializeVideoCapture();
            }
            return 0;
        }

        public int Capture_Close()
        {
            if (videoCaptureForm != null)
            {
                videoCaptureForm.StopCapture();
                videoCaptureForm.VideoCapture1 = null;
                videoCaptureForm.Close();
                videoCaptureForm = null;
            }
            return 0;
        }

        public int Capture_DeviceTableUpdate()
        {
            if (videoCaptureForm == null) { return -1; }
            return 0;
        }

        public int Capture_DeviceTableGetCount(int eType)
        {
            if ((eType < 0) || (eType > 1)) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();

            CAPTUREDEVICETYPE captureType = (CAPTUREDEVICETYPE)eType;
            List<string> captureDevices = this.GetCaptureDevices(captureType);
            return captureDevices.Count;
        }

        public int Capture_DeviceTableGetName(int eType, int iDeviceIndex, ulong ulpBstrName, int iLimit)
        {
            if ((eType < 0) || (eType > 1)) { return -1; }
            if (iDeviceIndex < 0) { return -1; }
            if (ulpBstrName == 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            CAPTUREDEVICETYPE captureType = (CAPTUREDEVICETYPE)eType;
            List<string> captureDevices = captureDevices = this.GetCaptureDevices(captureType);
            if (iDeviceIndex >= captureDevices.Count) { return 0; }
            string captureDeviceName = captureDevices[iDeviceIndex];
            int length = CopyStringToCPlus(captureDeviceName, ulpBstrName, iLimit);
            return length;
        }

        public int Capture_DeviceTableSearchName(int eType, ulong ulpBstrName, int iSize)
        {
            if ((eType < 0) || (eType > 1)) { return -1; }
            if (ulpBstrName == 0) { return -1; }
            if (iSize <= 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            CAPTUREDEVICETYPE captureType = (CAPTUREDEVICETYPE)eType;
            List<string> captureDevices = this.GetCaptureDevices(captureType);
            string captureDeviceName = CopyStringFromCPlus(ulpBstrName, iSize);
            int index = captureDevices.IndexOf(captureDeviceName);
            return index;
        }

        public int Capture_DeviceFormatTableGetCount(int eType, int iDeviceIndex)
        {
            if ((eType < 0) || (eType > 1)) { return -1; }
            if (iDeviceIndex < 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            CAPTUREDEVICETYPE captureType = (CAPTUREDEVICETYPE)eType;
            List<string> captureDevices = this.GetCaptureDevices(captureType);
            if (iDeviceIndex >= captureDevices.Count) { return 0; }
            List<string> captureDeviceFormats = this.GetCaptureDeviceFormats(captureType, iDeviceIndex);
            return captureDeviceFormats.Count;
        }

        public int Capture_DeviceFormatTableGetFormat(int eType, int iDeviceIndex, int iFormatIndex,
                                                      ulong ulpBstrFormat, int iLimit)
        {
            if ((eType < 0) || (eType > 1)) { return -1; }
            if ((iDeviceIndex < 0) || (iFormatIndex < 0)) { return -1; }
            if (ulpBstrFormat == 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            CAPTUREDEVICETYPE captureType = (CAPTUREDEVICETYPE)eType;
            List<string> captureDevices = this.GetCaptureDevices(captureType);
            if (iDeviceIndex >= captureDevices.Count) { return 0; }
            List<string> captureDeviceFormats = this.GetCaptureDeviceFormats(captureType, iDeviceIndex);
            if (iFormatIndex >= captureDeviceFormats.Count) { return 0; }
            string captureDeviceFormat = captureDeviceFormats[iFormatIndex];
            int length = CopyStringToCPlus(captureDeviceFormat, ulpBstrFormat, iLimit);
            return length;
        }

        public int Capture_VideoFrameRateTableGetCount(int iVideoIndex)
        {
            if (iVideoIndex < 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            List<string> captureDevices = this.GetCaptureDevices(CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_VIDEO);
            if (iVideoIndex >= captureDevices.Count) { return 0; }
            List<string> captureDeviceVideoFrameRates = this.GetCaptureDeviceVideoFrameRates(iVideoIndex);
            return captureDeviceVideoFrameRates.Count;
        }

        public int Capture_VideoFrameRateTableGetFrameRate(int iVideoIndex, int iFrameRateIndex,
                                                           ulong ulpBstrFrameRate, int iLimit)
        {
            if ((iVideoIndex < 0) || (iFrameRateIndex < 0)) { return -1; }
            if (ulpBstrFrameRate == 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            List<string> captureDevices = this.GetCaptureDevices(CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_VIDEO);
            if (iVideoIndex >= captureDevices.Count) { return 0; }
            List<string> captureDeviceVideoFrameRates = this.GetCaptureDeviceVideoFrameRates(iVideoIndex);
            if (iFrameRateIndex >= captureDeviceVideoFrameRates.Count) { return 0; }
            string captureDeviceVideoFrameRate = captureDeviceVideoFrameRates[iFrameRateIndex];
            int length = CopyStringToCPlus(captureDeviceVideoFrameRate, ulpBstrFrameRate, iLimit);
            return length;
        }

        public int Capture_DeviceOpen(int eSession, int iVideoIndex, int iAudioIndex)
        {
            if ( (eSession < 0) || (eSession > 2) ) { return -1; }
            if ((iVideoIndex < 0) || (iAudioIndex < 0)) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            List<string> audioCaptureDevices = this.GetCaptureDevices(CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_AUDIO);
            List<string> videoCaptureDevices = this.GetCaptureDevices(CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_VIDEO);
            CAPTURESESSION captureSession = (CAPTURESESSION)eSession;
            CaptureProfile profile = DefaultCaptureProfile();
            switch (captureSession)
            {
                case CAPTURESESSION.CAPTURESESSION_CAM:
                    if (iAudioIndex >= audioCaptureDevices.Count) { return -1; }
                    if (iVideoIndex >= videoCaptureDevices.Count) { return -1; }
                    profile.audioDevice = audioCaptureDevices[iAudioIndex];
                    profile.videoDevice = videoCaptureDevices[iVideoIndex];
                    break;
                case CAPTURESESSION.CAPTURESESSION_AUDIO:
                    if (iAudioIndex >= audioCaptureDevices.Count) { return -1; }
                    profile.audioDevice = audioCaptureDevices[iAudioIndex];
                    profile.videoDevice = "VisioForge Virtual Camera";
                    break;
                case CAPTURESESSION.CAPTURESESSION_SCREEN:
                    if (iAudioIndex >= audioCaptureDevices.Count) { return -1; }
                    profile.audioDevice = audioCaptureDevices[iAudioIndex];
                    break;
            }
            profile.captureSession = captureSession;
            videoCaptureForm.gCurrentProfile = profile;
            videoCaptureForm.Hide();
            return 0;
        }

        public int Capture_IPCameraOpen(ulong ulpBstrURL, int iURLSize,
                                        ulong ulpBstrLogin, int iLoginSize,
                                        ulong ulpBstrPassword, int iPasswordSize)
        {
            if ( (ulpBstrURL == 0) || (iURLSize <= 0) ) { return -1; }
            if (iLoginSize < 0) { return -1; }
            if ( (ulpBstrLogin == 0) && (iLoginSize > 0) ) { return -1; }
            if (iPasswordSize < 0) { return -1; }
            if ( (ulpBstrPassword == 0) && (iPasswordSize > 0) ) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            string url = CopyStringFromCPlus(ulpBstrURL, iURLSize);
            string login = null;
            if (iLoginSize > 0)
            {
                login = CopyStringFromCPlus(ulpBstrLogin, iLoginSize);
            }
            string password = null;
            if (iPasswordSize > 0)
            {
                password = CopyStringFromCPlus(ulpBstrPassword, iPasswordSize);
            }
            CaptureProfile profile = DefaultCaptureProfile();
            profile.ipCameraURL = url;
            profile.ipCameraLogin = login;
            profile.ipCameraPassword = password;
            profile.captureSession = CAPTURESESSION.CAPTURESESSION_IPCAM;
            if (videoCaptureForm.gDebug)
            {
                List<string> audioCaptureDevices = this.GetCaptureDevices(CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_AUDIO);
                profile.audioDevice = audioCaptureDevices[0];
            }
            videoCaptureForm.gCurrentProfile = profile;
            videoCaptureForm.Hide();
            return 0;
        }

        public int Capture_DeviceClose()
        {
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.Hide();
            videoCaptureForm.StopCapture();
            CaptureProfile profile = DefaultCaptureProfile();
            profile.captureSession = CAPTURESESSION.CAPTURESESSION_NONE;
            videoCaptureForm.gCurrentProfile = profile;
            return 0;
        }

        public int Capture_DisplayOpen(int xx, int yy, int lWidth, int lHeight)
        {
            if (videoCaptureForm == null) { return -1; }
            
            CaptureProfile profile = videoCaptureForm.gCurrentProfile;
            if (profile.captureSession == CAPTURESESSION.CAPTURESESSION_NONE)
            {
                return 0;
            }
            else if (profile.captureSession == CAPTURESESSION.CAPTURESESSION_SCREEN)
            {
                if (    (profile.displayShow == false)
                     && (profile.videoCaptureMode == VFVideoCaptureMode.ScreenPreview) )
                {
                    videoCaptureForm.Left = xx;
                    videoCaptureForm.Top = yy;
                    videoCaptureForm.Width = lWidth;
                    videoCaptureForm.Height = lHeight;
                    videoCaptureForm.VideoCapture1.Left = 0;
                    videoCaptureForm.VideoCapture1.Top = 0;
                    videoCaptureForm.VideoCapture1.Width = lWidth;
                    videoCaptureForm.VideoCapture1.Height = lHeight;
                    videoCaptureForm.Show();
                    profile.displayShow = true;
                }
                else
                {
                    videoCaptureForm.Hide();
                }
            }
            else if (profile.displayShow == false)
            {
                videoCaptureForm.Left = xx;
                videoCaptureForm.Top = yy;
                videoCaptureForm.Width = lWidth;
                videoCaptureForm.Height = lHeight;
                videoCaptureForm.VideoCapture1.Left = 0;
                videoCaptureForm.VideoCapture1.Top = 0;
                videoCaptureForm.VideoCapture1.Width = lWidth;
                videoCaptureForm.VideoCapture1.Height = lHeight;
                videoCaptureForm.Show();
                profile.displayShow = true;
            }
            else
            {
                videoCaptureForm.Show();
            }
            videoCaptureForm.gCurrentProfile = profile;
            return 0;
        }

        public int Capture_DisplayClose()
        {
            if (videoCaptureForm != null)
            {
                videoCaptureForm.Hide();
            }
            return 0;
        }

        public int Capture_PreviewPrepare()
        {
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            CaptureProfile profile = videoCaptureForm.gCurrentProfile;
            switch (profile.captureSession)
            {
                case CAPTURESESSION.CAPTURESESSION_NONE:
                    return -1;
                case CAPTURESESSION.CAPTURESESSION_CAM:
                    profile.videoCaptureMode = VFVideoCaptureMode.VideoPreview;
                    break;
                case CAPTURESESSION.CAPTURESESSION_AUDIO:
                    profile.videoCaptureMode = VFVideoCaptureMode.VideoPreview;
                    break;
                case CAPTURESESSION.CAPTURESESSION_SCREEN:
                    profile.videoCaptureMode = VFVideoCaptureMode.ScreenPreview;
                    break;
                case CAPTURESESSION.CAPTURESESSION_IPCAM:
                    profile.videoCaptureMode = VFVideoCaptureMode.IPPreview;
                    break;
            }
            videoCaptureForm.gCurrentProfile = profile;
            return 0;
        }

        public int Capture_CapturePrepare(ulong ulpBstrFile, int iFileSize,
                                          ulong ulpBstrProfile, int iProfileSize,
                                          int iAudioTimeshift, int iSyncView)
        {
            if (ulpBstrFile == 0) { return -1; }
            if (iFileSize <= 0) { return -1; }
            if (ulpBstrProfile == 0) { return -1; }
            if (iProfileSize <= 0) { return -1; }
            if (videoCaptureForm == null) { return -1; }
            videoCaptureForm.StopCapture();
            
            string profileName = CopyStringFromCPlus(ulpBstrProfile, iProfileSize);
            if (!File.Exists(profileName)) { return -1; }
            string fileName = CopyStringFromCPlus(ulpBstrFile, iFileSize);
            if (File.Exists(fileName)) { File.Delete(fileName); }
            
            CaptureProfile profile = videoCaptureForm.gCurrentProfile;
            profile.outputFilename = fileName;
            switch (profile.captureSession)
            {
                case CAPTURESESSION.CAPTURESESSION_NONE:
                    return -1;
                case CAPTURESESSION.CAPTURESESSION_CAM:
                    profile = ParseProfileFile(profileName, profile, "[Profile]");
                    profile.videoCaptureMode = VFVideoCaptureMode.VideoCapture;
                    profile.videoProfile = VFH264Profile.ProfileMain;
                    break;
                case CAPTURESESSION.CAPTURESESSION_AUDIO:
                    profile = ParseProfileFile(profileName, profile, "[Profile]");
                    profile.videoCaptureMode = VFVideoCaptureMode.VideoCapture;
                    profile.videoProfile = VFH264Profile.ProfileMain;
                    profile.videoBitRate = "128";
                    profile.videoFrameRate = "5";
                    profile.keyFramePerSecond = "30";
                    break;
                case CAPTURESESSION.CAPTURESESSION_SCREEN:
                    profile = ParseProfileFile(profileName, profile, "[ScreenProfile]");
                    profile.videoCaptureMode = VFVideoCaptureMode.ScreenCapture;
                    profile.videoProfile = VFH264Profile.ProfileBaseline;
                    videoCaptureForm.Hide();
                    break;
                case CAPTURESESSION.CAPTURESESSION_IPCAM:
                    profile = ParseProfileFile(profileName, profile, "[Profile]");
                    profile.videoCaptureMode = VFVideoCaptureMode.IPCapture;
                    profile.videoProfile = VFH264Profile.ProfileMain;
                    break;
            }
            profile.audioTimeshift = iAudioTimeshift;
            videoCaptureForm.gCurrentProfile = profile;
            if (iSyncView != 0)
            {
                videoCaptureForm.Show();
            }
            else
            {
                videoCaptureForm.Hide();
            }
            return 0;
        }

        public int Capture_DoStart()
        {
            if (videoCaptureForm == null) { return -1; }

            CaptureProfile profile = videoCaptureForm.gCurrentProfile;
            if (profile.captureSession == CAPTURESESSION.CAPTURESESSION_NONE) { return -1; }
            if (videoCaptureForm.VideoCapture1.Status != VFVideoCaptureStatus.Work)
            {
                videoCaptureForm.VideoCaptureFill();
                videoCaptureForm.VideoCapture1.Start();
            }
            return 0;
        }

        public int Capture_DoStop()
        {
            if (videoCaptureForm != null)
            {
                videoCaptureForm.StopCapture();
            }
            return 0;
        }

        public int Capture_DoPause()
        {
            return 0;
        }

        public int Capture_DoResume()
        {
            return 0;
        }

        public int Capture_GetCount()
        {
            int durationTime = videoCaptureForm.VideoCapture1.Duration_Time();
            return durationTime; // in milliseconds
        }

        public int Capture_GetRecordVolumeLevel()
        {
            return 100;
        }

        public VFVideoCaptureClass()
        {
            this.Hide();
            videoCaptureForm = this;
            this.InitializeComponent();
            this.InitializeVideoCapture();
            this.gCurrentProfile = new CaptureProfile();
        }

        protected override void WndProc(ref Message m)
        {
            const int WM_SYSCOMMAND = 0x112;
            const long SC_CLOSE = 0xF060L;

            if (m.Msg == WM_SYSCOMMAND &&
                (m.WParam.ToInt64() & 0xFFF0L) == SC_CLOSE)
            {
                videoCaptureForm.Capture_DisplayClose();
            }
            else
            {
                base.WndProc(ref m);
            }
        }

        protected override void OnResize(EventArgs e)
        {
        }

        private void VisioForgeVideoCaptureSDK_ResizeBegin(object sender, EventArgs e)
        {
            videoCaptureForm.SuspendLayout();
        }

        private void VisioForgeVideoCaptureSDK_ResizeEnd(object sender, EventArgs e)
        {
            videoCaptureForm.VideoCapture1.Width = videoCaptureForm.Width;
            videoCaptureForm.VideoCapture1.Height = videoCaptureForm.Height;
            videoCaptureForm.ResumeLayout();
        }

        private void StopCapture()
        {
            if (videoCaptureForm.VideoCapture1 != null)
            {
                if (videoCaptureForm.VideoCapture1.Status == VFVideoCaptureStatus.Work)
                {
                    videoCaptureForm.VideoCapture1.Stop();
                }
            }
        }

        private List<string> GetCaptureDevices(CAPTUREDEVICETYPE captureType)
        {
            List<string> captureDevices = new List<string>();
            switch (captureType)
            {
                case CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_AUDIO:
                    captureDevices = this.GetAudioCaptureDevices();
                    break;
                case CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_VIDEO:
                    captureDevices = this.GetVideoCaptureDevices();
                    break;
            }
            return captureDevices;
        }

        private List<string> GetAudioCaptureDevices()
        {
            List<string> audioCaptureDevices = new List<string>();
            audioCaptureDevices = videoCaptureForm.VideoCapture1.Audio_CaptureDevices;
            audioCaptureDevices.Remove("VisioForge Network Streamer Audio Source");
            audioCaptureDevices.Remove("VisioForge Virtual Audio Card");
            audioCaptureDevices.Remove("VisioForge What You Hear Source");
            audioCaptureDevices.Sort();
            return audioCaptureDevices;
        }

        private List<string> GetVideoCaptureDevices()
        {
            List<string> videoCaptureDevices = new List<string>();
            videoCaptureDevices = videoCaptureForm.VideoCapture1.Video_CaptureDevices;
            videoCaptureDevices.Remove("VisioForge Network Source Video");
            videoCaptureDevices.Remove("VisioForge Virtual Camera");
            videoCaptureDevices.Sort();
            return videoCaptureDevices;
        }

        private List<string> GetCaptureDeviceFormats(CAPTUREDEVICETYPE captureType, int iDeviceIndex)
        {
            List<string> captureDeviceFormats = new List<string>();
            switch (captureType)
            {
                case CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_AUDIO:
                    videoCaptureForm.VideoCapture1.Audio_CaptureDevice = videoCaptureForm.VideoCapture1.Audio_CaptureDevices[iDeviceIndex];
                    videoCaptureForm.VideoCapture1.Audio_CaptureDevice_Formats_Fill();
                    captureDeviceFormats = this.VideoCapture1.Audio_CaptureDevice_Formats();
                    break;
                case CAPTUREDEVICETYPE.CAPTUREDEVICETYPE_VIDEO:
                    videoCaptureForm.VideoCapture1.Video_CaptureDevice = videoCaptureForm.VideoCapture1.Video_CaptureDevices[iDeviceIndex];
                    videoCaptureForm.VideoCapture1.Video_CaptureDevice_Formats_Fill();
                    captureDeviceFormats = this.VideoCapture1.Video_CaptureDevice_Formats();
                    break;
            }
            return captureDeviceFormats;
        }

        private List<string> GetCaptureDeviceVideoFrameRates(int iDeviceIndex)
        {
            List<string> captureDeviceVideoFrameRates = new List<string>();
            videoCaptureForm.VideoCapture1.Video_CaptureDevice = videoCaptureForm.VideoCapture1.Video_CaptureDevices[iDeviceIndex];
            videoCaptureForm.VideoCapture1.Video_CaptureDevice_FrameRates_Fill();
            captureDeviceVideoFrameRates = this.VideoCapture1.Video_CaptureDevice_FrameRates();
            return captureDeviceVideoFrameRates;
        }

        private int CopyStringToCPlus(string bstr, ulong pBSTR, int limit)
        {
            int length = 0;
            if (bstr != null)
            {
                IntPtr intPtr = (IntPtr)pBSTR;
                char[] charArray = bstr.ToCharArray();
                length = charArray.Length;
                if (charArray.Length < limit)
                {
                    Marshal.Copy(charArray, 0, intPtr, length);
                }
            }
            return length;
        }

        private string CopyStringFromCPlus(ulong pBSTR, int size)
        {
            string bstr = null;
            if (size > 0)
            {
                IntPtr intPtr = (IntPtr)pBSTR;
                char[] charArray = new char[size];
                Marshal.Copy(intPtr, charArray, 0, size);
                bstr = new string(charArray);
            }
            return bstr;
        }

        private CaptureProfile ParseProfileFile(string profileName, CaptureProfile profile, string title)
        {
            string line = null;
            int lastLocation;

            System.IO.StreamReader profileFile = new System.IO.StreamReader(profileName, System.Text.Encoding.GetEncoding("shift_jis"));
            while ((line = profileFile.ReadLine()) != null)
            {
                if (line.Contains(title))
                {
                    break;
                }
            }
            while ((line = profileFile.ReadLine()) != null)
            {
                line.Trim();
                lastLocation = line.IndexOf("=");
                if (line.StartsWith("Param1="))
                {
                    profile.videoBitRate = line.Substring(lastLocation + 1);
                }
                else if (line.StartsWith("Param2="))
                {
                    profile.videoFrameRate = line.Substring(lastLocation + 1);
                }
                else if (line.StartsWith("Param3="))
                {
                    profile.keyFramePerSecond = line.Substring(lastLocation + 1);
                }
                else if (line.StartsWith("Param4="))
                {
                    profile.audioBitRate = line.Substring(lastLocation + 1);
                }
                else if (line.StartsWith("Param5="))
                {
                    profile.audioFormat = line.Substring(lastLocation + 1);
                }
                else if (line.StartsWith("Param6="))
                {
                    if (title == "[Profile]")
                    {
                        profile.videoFormat = line.Substring(lastLocation + 1);
                    }
                    else
                    {
                        if (line.Substring(lastLocation + 1) == "true")
                        {
                            profile.grabMouseCursor = true;
                        }
                        else
                        {
                            profile.grabMouseCursor = false;
                        }
                    }
                }
                else if (line.StartsWith("[ScreenProfile]"))
                {
                    break;
                }
                else if (line.StartsWith("[General]"))
                {
                    break;
                }
                else if (line.StartsWith("[Information]"))
                {
                    break;
                }
            }
            profileFile.Close();
            return profile;
        }

        private CaptureProfile DefaultCaptureProfile()
        {
            CaptureProfile captureProfile = new CaptureProfile();
            captureProfile.videoBitRate = "32";
            captureProfile.videoFrameRate = null;
            captureProfile.keyFramePerSecond = "3";
            captureProfile.audioBitRate = "32";
            captureProfile.audioFormat = null;
            captureProfile.audioProfile = VFAACObject.Low;
            captureProfile.videoProfile = VFH264Profile.ProfileBaseline;
            captureProfile.videoLevel = VFH264Level.LevelAuto;
            captureProfile.videoFormat = null;
            captureProfile.grabMouseCursor = false;
            captureProfile.audioTimeshift = 0;
            captureProfile.ipCameraURL = null;
            captureProfile.ipCameraLogin = null;
            captureProfile.ipCameraPassword = null;
            captureProfile.videoDevice = null;
            captureProfile.audioDevice = null;
            captureProfile.outputFilename = null;
            captureProfile.videoCaptureMode = VFVideoCaptureMode.VideoPreview;
            captureProfile.captureSession = CAPTURESESSION.CAPTURESESSION_NONE;
            captureProfile.captureState = CAPTURESTATE.CAPTURESTATE_Stoped;
            captureProfile.displayShow = false;
            return captureProfile;
        }

        private void VideoCaptureFill()
        {
            videoCaptureForm.VideoCapture1.SetLicenseKey("20B3-64EE-1ED9-74C7-6C61-6437",
                                                         "tbi_user",
                                                         "itoh@t-broad.com");

            videoCaptureForm.VideoCapture1.Debug_Mode = false;
            videoCaptureForm.VideoCapture1.IP_Camera_Debug_Enabled = false;
            videoCaptureForm.VideoCapture1.Video_Effects_Clear();
            videoCaptureForm.VideoCapture1.Network_Streaming_Enabled = false;
            videoCaptureForm.VideoCapture1.Audio_VUMeter_Enabled = false;
            videoCaptureForm.VideoCapture1.Audio_VUMeter_Pro_Enabled = false;
            videoCaptureForm.VideoCapture1.MultiScreen_Clear();
            videoCaptureForm.VideoCapture1.MultiScreen_Enabled = false;
            videoCaptureForm.VideoCapture1.Virtual_Camera_Output_Enabled = false;
            videoCaptureForm.VideoCapture1.Decklink_Output_Enabled = false;
            videoCaptureForm.VideoCapture1.AForge_Motion_Detection_Enabled = false;
            videoCaptureForm.VideoCapture1.FaceTracking_Enabled = false;
            videoCaptureForm.VideoCapture1.Barcode_Reader_Enabled = false;
            videoCaptureForm.VideoCapture1.OpenCL_Enabled = false;
            videoCaptureForm.VideoCapture1.ChromaKey_Enabled = false;
            videoCaptureForm.VideoCapture1.Video_Renderer = VisioForge.Types.VFVideoRenderer.EVR;
            videoCaptureForm.VideoCapture1.Video_Renderer_Deinterlace_UseDefault = true;
            videoCaptureForm.VideoCapture1.Video_Renderer_BackgroundColor = System.Drawing.Color.Black;
            videoCaptureForm.VideoCapture1.Video_Renderer_Deinterlace_Mode = "";
            videoCaptureForm.VideoCapture1.Video_Renderer_Flip_Horizontal = false;
            videoCaptureForm.VideoCapture1.Video_Renderer_Flip_Vertical = false;
            videoCaptureForm.VideoCapture1.Video_Renderer_RotationAngle = 0;
            videoCaptureForm.VideoCapture1.Video_Renderer_Zoom_Ratio = 0;
            videoCaptureForm.VideoCapture1.Video_Renderer_Zoom_ShiftX = 0;
            videoCaptureForm.VideoCapture1.Video_Renderer_Zoom_ShiftY = 0;
            videoCaptureForm.VideoCapture1.Video_Resize_LetterBox = false;
            videoCaptureForm.VideoCapture1.Video_Resize_Mode = VisioForge.Types.VFResizeMode.NearestNeighbor;
            videoCaptureForm.VideoCapture1.Video_Resize_NewHeight = 0;
            videoCaptureForm.VideoCapture1.Video_Resize_NewWidth = 0;
            videoCaptureForm.VideoCapture1.Video_ResizeOrCrop_Enabled = false;
            videoCaptureForm.VideoCapture1.Video_Rotation = VisioForge.Types.VFRotateMode.RotateNone;
            videoCaptureForm.VideoCapture1.Video_Sample_Grabber_Enabled = false;
            videoCaptureForm.VideoCapture1.Video_Sample_Grabber_UseForVideoEffects = false;
            videoCaptureForm.VideoCapture1.Video_Still_Frames_Grabber_Enabled = false;
            videoCaptureForm.VideoCapture1.MPEG_Video_Decoder = string.Empty;
            videoCaptureForm.VideoCapture1.MPEG_Audio_Decoder = string.Empty;
            videoCaptureForm.VideoCapture1.MotionDetection_Enabled = false;
            videoCaptureForm.VideoCapture1.Audio_Effects_Enabled = false;
            videoCaptureForm.VideoCapture1.SeparateCapture_Enabled = false;

            CaptureProfile profile = videoCaptureForm.gCurrentProfile;
            videoCaptureForm.VideoCapture1.Mode = profile.videoCaptureMode;
            
            videoCaptureForm.VideoCapture1.Audio_CaptureDevice = profile.audioDevice;
            videoCaptureForm.VideoCapture1.Audio_CaptureDevice_Format = profile.audioFormat;
            videoCaptureForm.VideoCapture1.Audio_CaptureDevice_Line = null;
            videoCaptureForm.VideoCapture1.Audio_OutputDevice = "Default DirectSound Device";
            videoCaptureForm.VideoCapture1.Additional_Audio_CaptureDevice_MixChannels = false;

            videoCaptureForm.VideoCapture1.Audio_RecordAudio = true;
            if (videoCaptureForm.gDebug)
            {
                videoCaptureForm.VideoCapture1.Audio_PlayAudio = true;
            }
            else
            {
                videoCaptureForm.VideoCapture1.Audio_PlayAudio = false;
            }
            
            videoCaptureForm.VideoCapture1.Video_CaptureDevice = profile.videoDevice;
            videoCaptureForm.VideoCapture1.Video_CaptureDevice_Format = profile.videoFormat;
            if (profile.videoFrameRate != null)
            {
                videoCaptureForm.VideoCapture1.Video_CaptureDevice_FrameRate = Convert.ToDouble(profile.videoFrameRate);
            }
            
            if (profile.videoFrameRate != null)
            {
                videoCaptureForm.VideoCapture1.Screen_Capture_FrameRate = (float)Convert.ToDouble(profile.videoFrameRate);
            }

            if (profile.captureSession == CAPTURESESSION.CAPTURESESSION_SCREEN)
            {
                videoCaptureForm.VideoCapture1.Screen_Capture_Mode = VisioForge.Types.VFScreenCaptureMode.Screen;
                videoCaptureForm.VideoCapture1.Screen_Capture_Grab_Mouse_Cursor = profile.grabMouseCursor;
                videoCaptureForm.VideoCapture1.Screen_Capture_FullScreen = false;
                videoCaptureForm.VideoCapture1.Screen_Capture_Left   = 0;
                videoCaptureForm.VideoCapture1.Screen_Capture_Top    = 0;
                int width = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Width;
                videoCaptureForm.VideoCapture1.Screen_Capture_Right  = width  & (int)(0xFFF0);
                int height = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Height;
                videoCaptureForm.VideoCapture1.Screen_Capture_Bottom = height & (int)(0xFFF0);
                videoCaptureForm.VideoCapture1.Screen_Capture_Display_Index = 1;
            }

            if (profile.audioTimeshift != 0)
            {
                videoCaptureForm.VideoCapture1.Audio_Enhancer_Enabled = true;
                videoCaptureForm.VideoCapture1.Audio_Enhancer_Timeshift(profile.audioTimeshift);
            }
            else
            {
                videoCaptureForm.VideoCapture1.Audio_Enhancer_Enabled = false;
            }

            if (profile.captureSession == CAPTURESESSION.CAPTURESESSION_AUDIO)
            {
                videoCaptureForm.VideoCapture1.Video_Effects_Enabled = true;
                videoCaptureForm.VideoCapture1.Video_Effect_Ex(14, 0, 0, true, VFVideoEffectType.Darkness, 100);
            }
            else
            {
                videoCaptureForm.VideoCapture1.Video_Effects_Enabled = false;
            }

            videoCaptureForm.VideoCapture1.IP_Camera_URL = profile.ipCameraURL;
            videoCaptureForm.VideoCapture1.IP_Camera_Login = profile.ipCameraLogin;
            videoCaptureForm.VideoCapture1.IP_Camera_Password = profile.ipCameraPassword;
            videoCaptureForm.VideoCapture1.IP_Camera_Type = VisioForge.Types.VFIPSource.Auto_VLC;
            if (videoCaptureForm.gDebug)
            {
                videoCaptureForm.VideoCapture1.IP_Camera_Audio_Capture = false;
            }
            else
            {
                videoCaptureForm.VideoCapture1.IP_Camera_Audio_Capture = true;
            }
            
            videoCaptureForm.VideoCapture1.Output_Filename = profile.outputFilename;
            videoCaptureForm.VideoCapture1.Output_Format = VFVideoCaptureOutputFormat.MP4;
            
            videoCaptureForm.VideoCapture1.MP4_LegacyCodecs = true;
            videoCaptureForm.VideoCapture1.MP4_Video_Profile = profile.videoProfile;
            videoCaptureForm.VideoCapture1.MP4_Video_Level = profile.videoLevel;
            videoCaptureForm.VideoCapture1.MP4_Video_Target_Usage = VFH264TargetUsage.BestQuality;
            videoCaptureForm.VideoCapture1.MP4_Video_PictureType  = VFH264PictureType.Auto;
            videoCaptureForm.VideoCapture1.MP4_Video_GOP = false;
            if (profile.keyFramePerSecond != null)
            {
                videoCaptureForm.VideoCapture1.MP4_Video_IDR_Period = Convert.ToInt32(profile.keyFramePerSecond);
            }
            videoCaptureForm.VideoCapture1.MP4_Video_P_Period = 1;
            if (profile.videoBitRate != null)
            {
                videoCaptureForm.VideoCapture1.MP4_Video_Bitrate = Convert.ToInt32(profile.videoBitRate);
                videoCaptureForm.VideoCapture1.MP4_Video_MaxBitrate = videoCaptureForm.VideoCapture1.MP4_Video_Bitrate * 2;
                videoCaptureForm.VideoCapture1.MP4_Video_MinBitrate = videoCaptureForm.VideoCapture1.MP4_Video_Bitrate / 2;
            }
            videoCaptureForm.VideoCapture1.MP4_Video_RateControl = VFH264RateControl.CBR;
            videoCaptureForm.VideoCapture1.MP4_Video_MBEncoding = VFH264MBEncoding.CAVLC;
            videoCaptureForm.VideoCapture1.Video_Renderer_Stretch = true;
            
            if (profile.audioBitRate != null)
            {
                videoCaptureForm.VideoCapture1.MP4_Audio_AAC_Bitrate = Convert.ToInt32(profile.audioBitRate);
            }
            videoCaptureForm.VideoCapture1.MP4_Audio_AAC_Version = VFAACVersion.MPEG4;
            videoCaptureForm.VideoCapture1.MP4_Audio_AAC_Object = profile.audioProfile;
            videoCaptureForm.VideoCapture1.MP4_Audio_AAC_Output = VFAACOutput.RAW;

            videoCaptureForm.VideoCapture1.Audio_CaptureDevice_Format_UseBest = false;
            videoCaptureForm.VideoCapture1.Video_CaptureDevice_Format_UseBest = false;
            videoCaptureForm.VideoCapture1.Audio_Use_Compression = false;
            videoCaptureForm.VideoCapture1.Video_Use_Compression = false;
            videoCaptureForm.VideoCapture1.Video_Use_Compression_DecodeUncompressedToRGB = false;

            VideoCapture1.Audio_CaptureDevice_SetVolume(100, false);
            VideoCapture1.Audio_OutputDevice_SetVolume(100);
        }

        private void InitializeVideoCapture()
        {
            this.VideoCapture1 = new VisioForge.Controls.WinForms.VideoCapture();
            this.SuspendLayout();
            this.Controls.Add(this.VideoCapture1);
            this.ResumeLayout(false);

            this.VideoCapture1.AForge_Motion_Detection_DetectorType = VisioForge.Types.AFMotionDetectorType.TwoFramesDifference;
            this.VideoCapture1.AForge_Motion_Detection_DifferenceThreshold = 15;
            this.VideoCapture1.AForge_Motion_Detection_Enabled = false;
            this.VideoCapture1.AForge_Motion_Detection_GridHeight = 16;
            this.VideoCapture1.AForge_Motion_Detection_GridWidth = 16;
            this.VideoCapture1.AForge_Motion_Detection_HighlightColor = System.Drawing.Color.Red;
            this.VideoCapture1.AForge_Motion_Detection_HighlightMotionGrid = true;
            this.VideoCapture1.AForge_Motion_Detection_HighlightMotionRegions = true;
            this.VideoCapture1.AForge_Motion_Detection_KeepObjectsEdges = false;
            this.VideoCapture1.AForge_Motion_Detection_MinObjectsHeight = 10;
            this.VideoCapture1.AForge_Motion_Detection_MinObjectsWidth = 10;
            this.VideoCapture1.AForge_Motion_Detection_MotionAmountToHighlight = 0.15F;
            this.VideoCapture1.AForge_Motion_Detection_ProcessorType = VisioForge.Types.AFMotionProcessorType.MotionAreaHighlighting;
            this.VideoCapture1.AForge_Motion_Detection_SuppressNoise = true;
            this.VideoCapture1.Audio_CaptureDevice = "";
            this.VideoCapture1.Audio_CaptureDevice_CustomLatency = 0;
            this.VideoCapture1.Audio_CaptureDevice_Format = "";
            this.VideoCapture1.Audio_CaptureDevice_Format_UseBest = false;
            this.VideoCapture1.Audio_CaptureDevice_Line = "";
            this.VideoCapture1.Audio_Codec_BPS = 16;
            this.VideoCapture1.Audio_Codec_Channels = 2;
            this.VideoCapture1.Audio_Codec_Name = "";
            this.VideoCapture1.Audio_Codec_SampleRate = 44100;
            this.VideoCapture1.Audio_Decoder = null;
            this.VideoCapture1.Audio_Effects_Enabled = false;
            this.VideoCapture1.Audio_OutputDevice = "Default DirectSound Device";
            this.VideoCapture1.Audio_PlayAudio = true;
            this.VideoCapture1.Audio_RecordAudio = true;
            this.VideoCapture1.Audio_Sample_Grabber_Enabled = false;
            this.VideoCapture1.Audio_Use_Compression = false;
            this.VideoCapture1.Audio_VUMeter_Enabled = false;
            this.VideoCapture1.Audio_VUMeter_Pro_Enabled = false;
            this.VideoCapture1.Audio_VUMeter_Pro_Volume = 100;
            this.VideoCapture1.BackColor = System.Drawing.Color.Black;
            this.VideoCapture1.Barcode_Reader_Enabled = false;
            this.VideoCapture1.Barcode_Reader_Type = VisioForge.Types.VFBarcodeType.QR;
            this.VideoCapture1.BDA_Receiver_Name = null;
            this.VideoCapture1.BDA_Source_Name = null;
            this.VideoCapture1.BDA_SourceType = VisioForge.Types.VFBDAType.DVBT;
            this.VideoCapture1.ChromaKey_Color = VisioForge.Types.VFChromaColor.Green;
            this.VideoCapture1.ChromaKey_ContrastHigh = 70;
            this.VideoCapture1.ChromaKey_ContrastLow = 110;
            this.VideoCapture1.ChromaKey_Enabled = false;
            this.VideoCapture1.ChromaKey_ImageFilename = null;
            this.VideoCapture1.Custom_Output_Audio_Codec = "";
            this.VideoCapture1.Custom_Output_Audio_Codec_Use_Filters_Category = false;
            this.VideoCapture1.Custom_Output_Mux_Filter_Is_Encoder = false;
            this.VideoCapture1.Custom_Output_Mux_Filter_Name = "";
            this.VideoCapture1.Custom_Output_Special_FileWriter_Filter_Name = "";
            this.VideoCapture1.Custom_Output_Special_FileWriter_Needed = false;
            this.VideoCapture1.Custom_Output_Video_Codec = "";
            this.VideoCapture1.Custom_Output_Video_Codec_Use_Filters_Category = false;
            this.VideoCapture1.Debug_Dir = "";
            this.VideoCapture1.Debug_Mode = false;
            this.VideoCapture1.Decklink_Input = VisioForge.Types.DecklinkInput.Auto;
            this.VideoCapture1.Decklink_Input_Capture_Timecode_Source = VisioForge.Types.DecklinkCaptureTimecodeSource.Auto;
            this.VideoCapture1.Decklink_Input_IREUSA = false;
            this.VideoCapture1.Decklink_Input_SMPTE = false;
            this.VideoCapture1.Decklink_Output_AnalogOutput = VisioForge.Types.DecklinkAnalogOutput.Auto;
            this.VideoCapture1.Decklink_Output_AnalogOutputIREUSA = false;
            this.VideoCapture1.Decklink_Output_AnalogOutputSMPTE = false;
            this.VideoCapture1.Decklink_Output_BlackToDeckInCapture = VisioForge.Types.DecklinkBlackToDeckInCapture.Default;
            this.VideoCapture1.Decklink_Output_DualLinkOutputMode = VisioForge.Types.DecklinkDualLinkMode.Default;
            this.VideoCapture1.Decklink_Output_DV_Encoding = false;
            this.VideoCapture1.Decklink_Output_Enabled = false;
            this.VideoCapture1.Decklink_Output_HDTVPulldownOnOutput = VisioForge.Types.DecklinkHDTVPulldownOnOutput.Default;
            this.VideoCapture1.Decklink_Output_SingleFieldOutputForSynchronousFrames = VisioForge.Types.DecklinkSingleFieldOutputForSynchronousFrames.Default;
            this.VideoCapture1.Decklink_Output_VideoOutputDownConversionMode = VisioForge.Types.DecklinkVideoOutputDownConversionMode.Auto;
            this.VideoCapture1.Decklink_Output_VideoOutputDownConversionModeAnalogUsed = false;
            this.VideoCapture1.DirectCapture_Muxer = null;
            this.VideoCapture1.DV_Capture_Audio_Channels = 2;
            this.VideoCapture1.DV_Capture_Audio_SampleRate = 44100;
            this.VideoCapture1.DV_Capture_Type2 = false;
            this.VideoCapture1.DV_Capture_Video_Format = VisioForge.Types.VFDVVideoFormat.PAL;
            this.VideoCapture1.DV_Decoder_Video_Resolution = VisioForge.Types.VFDVVideoResolution.Full;
            this.VideoCapture1.Encryption_Format = VisioForge.Types.VFEncryptionFormat.MP4_H264_SW_AAC;
            this.VideoCapture1.Encryption_LicenseKey = null;
            this.VideoCapture1.Encryption_Password = null;
            this.VideoCapture1.FaceTracking_ColorMode = VisioForge.Types.AFCamshiftMode.RGB;
            this.VideoCapture1.FaceTracking_Enabled = false;
            this.VideoCapture1.FaceTracking_Highlight = false;
            this.VideoCapture1.FaceTracking_MinimumWindowSize = 0;
            this.VideoCapture1.FaceTracking_ScaleFactor = 0F;
            this.VideoCapture1.FaceTracking_ScalingMode = VisioForge.Types.AFObjectDetectorScalingMode.GreaterToSmaller;
            this.VideoCapture1.FaceTracking_SearchMode = VisioForge.Types.AFObjectDetectorSearchMode.Default;
            this.VideoCapture1.FFMPEG_AspectRatioH = 3;
            this.VideoCapture1.FFMPEG_AspectRatioW = 4;
            this.VideoCapture1.FFMPEG_AudioBitrate = 128000;
            this.VideoCapture1.FFMPEG_AudioChannels = 2;
            this.VideoCapture1.FFMPEG_AudioSampleRate = 44100;
            this.VideoCapture1.FFMPEG_Interlace = false;
            this.VideoCapture1.FFMPEG_OutputFormat = VisioForge.Types.VFFFMPEGOutputFormat.FLV;
            this.VideoCapture1.FFMPEG_TVSystem = VisioForge.Types.VFFFMPEGTVSystem.None;
            this.VideoCapture1.FFMPEG_VideoBitrate = 9000;
            this.VideoCapture1.FFMPEG_VideoBufferSize = 0;
            this.VideoCapture1.FFMPEG_VideoHeight = 480;
            this.VideoCapture1.FFMPEG_VideoMaxBitrate = 0;
            this.VideoCapture1.FFMPEG_VideoMinBitrate = 0;
            this.VideoCapture1.FFMPEG_VideoWidth = 640;
            this.VideoCapture1.FLAC_AdaptiveMidSideCoding = false;
            this.VideoCapture1.FLAC_BlockSize = 4608;
            this.VideoCapture1.FLAC_ExhaustiveModelSearch = true;
            this.VideoCapture1.FLAC_Level = 5;
            this.VideoCapture1.FLAC_LPCOrder = 8;
            this.VideoCapture1.FLAC_MidSideCoding = false;
            this.VideoCapture1.FLAC_RiceMax = 3;
            this.VideoCapture1.FLAC_RiceMin = 3;
            this.VideoCapture1.Frame_Save_Resize = false;
            this.VideoCapture1.Frame_Save_Resize_Height = 0;
            this.VideoCapture1.Frame_Save_Resize_Width = 0;
            this.VideoCapture1.Frame_Save_ZoomRatio = 0D;
            this.VideoCapture1.IP_Camera_Audio_Capture = false;
            this.VideoCapture1.IP_Camera_Debug_Enabled = false;
            this.VideoCapture1.IP_Camera_Debug_Filename = null;
            this.VideoCapture1.IP_Camera_Forced_Framerate = 0D;
            this.VideoCapture1.IP_Camera_Forced_Framerate_InstanceID = '\0';
            this.VideoCapture1.IP_Camera_Login = null;
            this.VideoCapture1.IP_Camera_Password = null;
            this.VideoCapture1.IP_Camera_Type = VisioForge.Types.VFIPSource.Auto_VLC;
            this.VideoCapture1.IP_Camera_URL = "";
            this.VideoCapture1.LAME_CBR_Bitrate = 192;
            this.VideoCapture1.LAME_Channels_Mode = VisioForge.Types.VFLameChannelsMode.StandardStereo;
            this.VideoCapture1.LAME_Copyright = false;
            this.VideoCapture1.LAME_CRC_Protected = false;
            this.VideoCapture1.LAME_Disable_Short_Blocks = false;
            this.VideoCapture1.LAME_Enable_Xing_VBR_Tag = false;
            this.VideoCapture1.LAME_Encoding_Quality = 10;
            this.VideoCapture1.LAME_Force_Mono = false;
            this.VideoCapture1.LAME_Keep_All_Frequencies = false;
            this.VideoCapture1.LAME_Mode_Fixed = false;
            this.VideoCapture1.LAME_Original = false;
            this.VideoCapture1.LAME_Sample_Rate = 44100;
            this.VideoCapture1.LAME_Strict_ISO_Compliance = false;
            this.VideoCapture1.LAME_Strictly_Enforce_VBR_Min_Bitrate = false;
            this.VideoCapture1.LAME_UseInAVI = false;
            this.VideoCapture1.LAME_VBR_Max_Bitrate = 192;
            this.VideoCapture1.LAME_VBR_Min_Bitrate = 128;
            this.VideoCapture1.LAME_VBR_Mode = false;
            this.VideoCapture1.LAME_VBR_Quality = 10;
            this.VideoCapture1.LAME_Voice_Encoding_Mode = false;
            this.VideoCapture1.Location = new System.Drawing.Point(0, 0);
            this.VideoCapture1.Mode = VisioForge.Types.VFVideoCaptureMode.VideoPreview;
            this.VideoCapture1.MotionDetection_Compare_Blue = false;
            this.VideoCapture1.MotionDetection_Compare_Green = false;
            this.VideoCapture1.MotionDetection_Compare_Greyscale = true;
            this.VideoCapture1.MotionDetection_Compare_Red = false;
            this.VideoCapture1.MotionDetection_DropFrames_Enabled = false;
            this.VideoCapture1.MotionDetection_DropFrames_Threshold = 5;
            this.VideoCapture1.MotionDetection_Enabled = false;
            this.VideoCapture1.MotionDetection_FrameInterval = 0;
            this.VideoCapture1.MotionDetection_Highlight_Color = VisioForge.Types.VFMotionCHLColor.Green;
            this.VideoCapture1.MotionDetection_Highlight_Enabled = false;
            this.VideoCapture1.MotionDetection_Highlight_Threshold = 25;
            this.VideoCapture1.MotionDetection_Matrix_Height = 8;
            this.VideoCapture1.MotionDetection_Matrix_Width = 8;
            this.VideoCapture1.MP4_Audio_AAC_Bitrate = 0;
            this.VideoCapture1.MP4_Audio_AAC_Object = VisioForge.Types.VFAACObject.Low;
            this.VideoCapture1.MP4_Audio_AAC_Output = VisioForge.Types.VFAACOutput.RAW;
            this.VideoCapture1.MP4_Audio_AAC_Version = VisioForge.Types.VFAACVersion.MPEG4;
            this.VideoCapture1.MP4_CUDA_Audio_AAC_Bitrate = 0;
            this.VideoCapture1.MP4_CUDA_Audio_AAC_Object = VisioForge.Types.VFAACObject.Low;
            this.VideoCapture1.MP4_CUDA_Audio_AAC_Output = VisioForge.Types.VFAACOutput.RAW;
            this.VideoCapture1.MP4_CUDA_Audio_AAC_Version = VisioForge.Types.VFAACVersion.MPEG4;
            this.VideoCapture1.MP4_CUDA_Video_Bitrate = 0;
            this.VideoCapture1.MP4_CUDA_Video_BitrateAuto = false;
            this.VideoCapture1.MP4_CUDA_Video_Deblocking = false;
            this.VideoCapture1.MP4_CUDA_Video_Deinterlace = false;
            this.VideoCapture1.MP4_CUDA_Video_Encoder = VisioForge.Types.VFCUDAVideoEncoder.Modern;
            this.VideoCapture1.MP4_CUDA_Video_FieldEncoding = VisioForge.Types.VFCUDAH264FieldEncoding.Auto;
            this.VideoCapture1.MP4_CUDA_Video_ForceGPU_ID = -1;
            this.VideoCapture1.MP4_CUDA_Video_ForceIDR = false;
            this.VideoCapture1.MP4_CUDA_Video_ForceIntra = false;
            this.VideoCapture1.MP4_CUDA_Video_GOP = false;
            this.VideoCapture1.MP4_CUDA_Video_IDR_Period = 0;
            this.VideoCapture1.MP4_CUDA_Video_Level = VisioForge.Types.VFCUDAH264LevelIDC.LevelAuto;
            this.VideoCapture1.MP4_CUDA_Video_MBEncoding = VisioForge.Types.VFCUDAH264MBEncoding.CAVLC;
            this.VideoCapture1.MP4_CUDA_Video_MultiGPU = true;
            this.VideoCapture1.MP4_CUDA_Video_P_Period = 0;
            this.VideoCapture1.MP4_CUDA_Video_Peak_Bitrate = 6000;
            this.VideoCapture1.MP4_CUDA_Video_Profile = VisioForge.Types.VFCUDAH264ProfileIDC.ProfileAuto;
            this.VideoCapture1.MP4_CUDA_Video_QPLevel_InterB = 28;
            this.VideoCapture1.MP4_CUDA_Video_QPLevel_InterP = 28;
            this.VideoCapture1.MP4_CUDA_Video_QPLevel_Intra = 28;
            this.VideoCapture1.MP4_CUDA_Video_RateControl = VisioForge.Types.VFCUDAH264RateControl.VBR;
            this.VideoCapture1.MP4_LegacyCodecs = false;
            this.VideoCapture1.MP4_Video_Bitrate = 0;
            this.VideoCapture1.MP4_Video_BitrateAuto = false;
            this.VideoCapture1.MP4_Video_Deblocking = false;
            this.VideoCapture1.MP4_Video_GOP = false;
            this.VideoCapture1.MP4_Video_IDR_Period = 1;
            this.VideoCapture1.MP4_Video_Level = VisioForge.Types.VFH264Level.LevelAuto;
            this.VideoCapture1.MP4_Video_MaxBitrate = 40000;
            this.VideoCapture1.MP4_Video_MBEncoding = VisioForge.Types.VFH264MBEncoding.CAVLC;
            this.VideoCapture1.MP4_Video_MinBitrate = 0;
            this.VideoCapture1.MP4_Video_P_Period = 1;
            this.VideoCapture1.MP4_Video_PictureType = VisioForge.Types.VFH264PictureType.Auto;
            this.VideoCapture1.MP4_Video_Profile = VisioForge.Types.VFH264Profile.ProfileBaseline;
            this.VideoCapture1.MP4_Video_RateControl = VisioForge.Types.VFH264RateControl.CBR;
            this.VideoCapture1.MP4_Video_Sequential_Timing = VisioForge.Types.VFH264TimeType.Default;
            this.VideoCapture1.MP4_Video_Target_Usage = VisioForge.Types.VFH264TargetUsage.BestQuality;
            this.VideoCapture1.MP4_Video_VBV_Buffer_Size = 0;
            this.VideoCapture1.MPEG_Audio_Decoder = "";
            this.VideoCapture1.MPEG_Video_Decoder = "";
            this.VideoCapture1.MultiScreen_Enabled = false;
            this.VideoCapture1.Name = "VideoCapture1";
            this.VideoCapture1.Network_Streaming_Audio_Enabled = false;
            this.VideoCapture1.Network_Streaming_Enabled = false;
            this.VideoCapture1.Network_Streaming_Format = VisioForge.Types.VFNetworkStreamingFormat.WMV;
            this.VideoCapture1.Network_Streaming_Network_Port = 10;
            this.VideoCapture1.Network_Streaming_URL = "";
            this.VideoCapture1.Network_Streaming_WMV_Maximum_Clients = 10;
            this.VideoCapture1.Network_Streaming_WMV_Profile_FileName = "";
            this.VideoCapture1.Network_Streaming_WMV_UseMain_WMV_Settings = false;
            this.VideoCapture1.OggVorbis_AvgBitRate = 128;
            this.VideoCapture1.OggVorbis_MaxBitRate = 192;
            this.VideoCapture1.OggVorbis_MinBitRate = 64;
            this.VideoCapture1.OggVorbis_Mode = VisioForge.Types.VFVorbisMode.Bitrate;
            this.VideoCapture1.OggVorbis_Quality = 80;
            this.VideoCapture1.OpenCL_Enabled = false;
            this.VideoCapture1.Output_Filename = "";
            this.VideoCapture1.Output_Format = VisioForge.Types.VFVideoCaptureOutputFormat.MP4;
            this.VideoCapture1.PIP_AddSampleGrabbers = false;
            this.VideoCapture1.PIP_Mode = VisioForge.Types.VFPIPMode.Custom;
            this.VideoCapture1.SBE_Encoder_CaptureMode = false;
            this.VideoCapture1.SBE_Encoder_Output_Format = VisioForge.Types.VFSBEOutputFormat.DV;
            // TODO: Code generation for '' failed because of Exception 'Invalid Primitive Type: System.IntPtr. Consider using CodeObjectCreateExpression.'.
            this.VideoCapture1.SBE_Player_Screen_Height = 0;
            this.VideoCapture1.SBE_Player_Screen_Width = 0;
            this.VideoCapture1.Screen_Capture_Bottom = 480;
            this.VideoCapture1.Screen_Capture_Display_Index = 0;
            this.VideoCapture1.Screen_Capture_FrameRate = 10D;
            this.VideoCapture1.Screen_Capture_FullScreen = true;
            this.VideoCapture1.Screen_Capture_Grab_Mouse_Cursor = true;
            this.VideoCapture1.Screen_Capture_Left = 0;
            this.VideoCapture1.Screen_Capture_Mode = VisioForge.Types.VFScreenCaptureMode.Screen;
            this.VideoCapture1.Screen_Capture_Right = 640;
            this.VideoCapture1.Screen_Capture_Top = 0;
            // TODO: Code generation for '' failed because of Exception 'Invalid Primitive Type: System.IntPtr. Consider using CodeObjectCreateExpression.'.
            this.VideoCapture1.SeparateCapture_AutostartCapture = false;
            this.VideoCapture1.SeparateCapture_Enabled = false;
            this.VideoCapture1.SeparateCapture_FileSizeThreshold = ((long)(0));
            this.VideoCapture1.SeparateCapture_Mode = VisioForge.Types.VFSeparateCaptureMode.Normal;
            this.VideoCapture1.SeparateCapture_TimeThreshold = ((long)(0));
            this.VideoCapture1.Size = new System.Drawing.Size(0, 0);
            this.VideoCapture1.Speex_BitRate = 48;
            this.VideoCapture1.Speex_BitrateControl = VisioForge.Types.SpeexBitrateControl.CBRQuality;
            this.VideoCapture1.Speex_Complexity = 3;
            this.VideoCapture1.Speex_MaxBitRate = 96;
            this.VideoCapture1.Speex_Mode = VisioForge.Types.SpeexEncodeMode.Auto;
            this.VideoCapture1.Speex_Quality = 8;
            this.VideoCapture1.Speex_UseAGC = false;
            this.VideoCapture1.Speex_UseDenoise = false;
            this.VideoCapture1.Speex_UseDTX = false;
            this.VideoCapture1.Speex_UseVAD = false;
            this.VideoCapture1.TabIndex = 77;
            this.VideoCapture1.TVTuner_Channel = 0;
            this.VideoCapture1.TVTuner_Country = "";
            this.VideoCapture1.TVTuner_FM_Tuning_StartFrequency = 80000000;
            this.VideoCapture1.TVTuner_FM_Tuning_Step = 160000000;
            this.VideoCapture1.TVTuner_FM_Tuning_StopFrequency = 0;
            this.VideoCapture1.TVTuner_Frequency = 0;
            this.VideoCapture1.TVTuner_InputType = "";
            this.VideoCapture1.TVTuner_Mode = "";
            this.VideoCapture1.TVTuner_Name = "";
            this.VideoCapture1.TVTuner_TVFormat = VisioForge.Types.VFTVTunerVideoFormat.PAL_D;
            this.VideoCapture1.Video_AVI_10_Compatibility = false;
            this.VideoCapture1.Video_AVI_Interleaving = VisioForge.Types.VFAVIInterleaving.None;
            this.VideoCapture1.Video_CaptureDevice = "";
            this.VideoCapture1.Video_CaptureDevice_Format = "";
            this.VideoCapture1.Video_CaptureDevice_Format_UseBest = false;
            this.VideoCapture1.Video_CaptureDevice_FrameRate = 0D;
            this.VideoCapture1.Video_CaptureDevice_InternalMPEGEncoder_Name = "";
            this.VideoCapture1.Video_CaptureDevice_IsAudioSource = false;
            this.VideoCapture1.Video_CaptureDevice_Path = null;
            this.VideoCapture1.Video_Codec = "";
            this.VideoCapture1.Video_Crop_Bottom = 0;
            this.VideoCapture1.Video_Crop_Left = 0;
            this.VideoCapture1.Video_Crop_Right = 0;
            this.VideoCapture1.Video_Crop_Top = 0;
            this.VideoCapture1.Video_Decoder = null;
            this.VideoCapture1.Video_Effects_Enabled = false;
            this.VideoCapture1.Video_Renderer = VisioForge.Types.VFVideoRenderer.EVR;
            this.VideoCapture1.Video_Renderer_BackgroundColor = System.Drawing.Color.Black;
            this.VideoCapture1.Video_Renderer_Deinterlace_Mode = "";
            this.VideoCapture1.Video_Renderer_Deinterlace_UseDefault = false;
            this.VideoCapture1.Video_Renderer_Flip_Horizontal = false;
            this.VideoCapture1.Video_Renderer_Flip_Vertical = false;
            this.VideoCapture1.Video_Renderer_RotationAngle = 0;
            this.VideoCapture1.Video_Renderer_Stretch = false;
            this.VideoCapture1.Video_Renderer_Zoom_Ratio = 0;
            this.VideoCapture1.Video_Renderer_Zoom_ShiftX = 0;
            this.VideoCapture1.Video_Renderer_Zoom_ShiftY = 0;
            this.VideoCapture1.Video_Resize_LetterBox = false;
            this.VideoCapture1.Video_Resize_Mode = VisioForge.Types.VFResizeMode.NearestNeighbor;
            this.VideoCapture1.Video_Resize_NewHeight = 0;
            this.VideoCapture1.Video_Resize_NewWidth = 0;
            this.VideoCapture1.Video_ResizeOrCrop_Enabled = false;
            this.VideoCapture1.Video_Rotation = VisioForge.Types.VFRotateMode.RotateNone;
            this.VideoCapture1.Video_Sample_Grabber_Enabled = true;
            this.VideoCapture1.Video_Sample_Grabber_UseForVideoEffects = true;
            this.VideoCapture1.Video_Still_Frames_Grabber_Enabled = false;
            this.VideoCapture1.Video_Use_Compression = false;
            this.VideoCapture1.Video_Use_Compression_DecodeUncompressedToRGB = false;
            this.VideoCapture1.Virtual_Camera_Output_Enabled = false;
            this.VideoCapture1.Virtual_Camera_Output_LicenseKey = null;
            this.VideoCapture1.VLC_Path = null;
            this.VideoCapture1.WebM_Audio_Quality = 80;
            this.VideoCapture1.WebM_Video_ARNR_MaxFrames = -1;
            this.VideoCapture1.WebM_Video_ARNR_Strength = -1;
            this.VideoCapture1.WebM_Video_ARNR_Type = -1;
            this.VideoCapture1.WebM_Video_AutoAltRef = false;
            this.VideoCapture1.WebM_Video_Bitrate = 2000;
            this.VideoCapture1.WebM_Video_CPUUsed = -17;
            this.VideoCapture1.WebM_Video_Decimate = -1;
            this.VideoCapture1.WebM_Video_DecoderBufferInitialSize = -1;
            this.VideoCapture1.WebM_Video_DecoderBufferOptimalSize = -1;
            this.VideoCapture1.WebM_Video_DecoderBufferSize = -1;
            this.VideoCapture1.WebM_Video_DropframeThreshold = -1;
            this.VideoCapture1.WebM_Video_EndUsage = VisioForge.Types.VP8EndUsageMode.Default;
            this.VideoCapture1.WebM_Video_ErrorResilient = false;
            this.VideoCapture1.WebM_Video_FixedKeyframeInterval = -1;
            this.VideoCapture1.WebM_Video_KeyframeMaxInterval = -1;
            this.VideoCapture1.WebM_Video_KeyframeMinInterval = -1;
            this.VideoCapture1.WebM_Video_KeyframeMode = VisioForge.Types.VP8KeyframeMode.Disabled;
            this.VideoCapture1.WebM_Video_LagInFrames = -1;
            this.VideoCapture1.WebM_Video_MaxQuantizer = -1;
            this.VideoCapture1.WebM_Video_MinQuantizer = -1;
            this.VideoCapture1.WebM_Video_Mode = VisioForge.Types.VP8QualityMode.Realtime;
            this.VideoCapture1.WebM_Video_OvershootPct = -1;
            this.VideoCapture1.WebM_Video_SpatialResamplingAllowed = false;
            this.VideoCapture1.WebM_Video_SpatialResamplingDownThreshold = -1;
            this.VideoCapture1.WebM_Video_SpatialResamplingUpThreshold = -1;
            this.VideoCapture1.WebM_Video_StaticThreshold = -1;
            this.VideoCapture1.WebM_Video_ThreadCount = 8;
            this.VideoCapture1.WebM_Video_TokenPartition = -1;
            this.VideoCapture1.WebM_Video_UndershootPct = -1;
            this.VideoCapture1.WMV_Custom_Audio_Codec = "";
            this.VideoCapture1.WMV_Custom_Audio_Format = "";
            this.VideoCapture1.WMV_Custom_Audio_LanguageID = "";
            this.VideoCapture1.WMV_Custom_Audio_Mode = VisioForge.Types.VFWMVStreamMode.CBR;
            this.VideoCapture1.WMV_Custom_Audio_PeakBitrate = 0;
            this.VideoCapture1.WMV_Custom_Audio_PeakBufferSize = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Audio_Quality = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Audio_StreamPresent = false;
            this.VideoCapture1.WMV_Custom_Profile_Description = "";
            this.VideoCapture1.WMV_Custom_Profile_Language = "";
            this.VideoCapture1.WMV_Custom_Profile_Name = "";
            this.VideoCapture1.WMV_Custom_Video_Bitrate = 0;
            this.VideoCapture1.WMV_Custom_Video_Buffer_Size = 0;
            this.VideoCapture1.WMV_Custom_Video_Buffer_UseDefault = false;
            this.VideoCapture1.WMV_Custom_Video_Codec = "";
            this.VideoCapture1.WMV_Custom_Video_FrameRate = 0D;
            this.VideoCapture1.WMV_Custom_Video_Height = 0;
            this.VideoCapture1.WMV_Custom_Video_KeyFrameInterval = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Video_Mode = VisioForge.Types.VFWMVStreamMode.CBR;
            this.VideoCapture1.WMV_Custom_Video_PeakBitRate = 0;
            this.VideoCapture1.WMV_Custom_Video_PeakBufferSizeSeconds = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Video_Quality = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Video_SizeSameAsInput = false;
            this.VideoCapture1.WMV_Custom_Video_Smoothness = ((byte)(0));
            this.VideoCapture1.WMV_Custom_Video_StreamPresent = false;
            this.VideoCapture1.WMV_Custom_Video_TVSystem = VisioForge.Types.VFWMVTVSystem.PAL;
            this.VideoCapture1.WMV_Custom_Video_Width = 0;
            this.VideoCapture1.WMV_External_Profile_File_Name = "";
            this.VideoCapture1.WMV_External_Profile_Text = null;
            this.VideoCapture1.WMV_Internal_Profile_Name = "Windows Media Video 9 for Local Network (768 kbps)";
            this.VideoCapture1.WMV_Mode = VisioForge.Types.VFWMVMode.ExternalProfile;
            this.VideoCapture1.WMV_V8_Profile_Name = "";

            this.VideoCapture1.Video_Effects_Clear();
            this.VideoCapture1.MultiScreen_Clear();
            this.VideoCapture1.Audio_CaptureDevice_SetVolume(100, false);
            this.VideoCapture1.Audio_OutputDevice_SetVolume(100);
        }
    }
}
