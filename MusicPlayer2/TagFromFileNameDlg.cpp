﻿// TagFromFileNameDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "TagFromFileNameDlg.h"
#include "afxdialogex.h"
#include "PropertyDlgHelper.h"


// CTagFromFileNameDlg 对话框

IMPLEMENT_DYNAMIC(CTagFromFileNameDlg, CDialog)

static const vector<wstring> default_formular{ FORMULAR_ARTIST L" - " FORMULAR_TITLE, FORMULAR_TITLE L" - " FORMULAR_ARTIST, FORMULAR_TRACK L"." FORMULAR_ARTIST L" - " FORMULAR_TITLE };

CTagFromFileNameDlg::CTagFromFileNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TAG_FROM_FILE_NAME_DIALOG, pParent)
{

}

CTagFromFileNameDlg::~CTagFromFileNameDlg()
{
}

void CTagFromFileNameDlg::SetDialogTitle(LPCTSTR str_title)
{
    m_dlg_title = str_title;
}

wstring CTagFromFileNameDlg::GetFormularSelected() const
{
    return m_formular_selected;
}

void CTagFromFileNameDlg::SaveConfig() const
{
    CIniHelper ini(theApp.m_config_path);
    ini.WriteBool(L"tag_edit", L"insert_when_clicked", IsInsertWhenClicked());
    ini.WriteStringList(L"tag_edit", L"default_formular", m_default_formular);
    ini.Save();
}

void CTagFromFileNameDlg::LoadConfig()
{
    CIniHelper ini(theApp.m_config_path);
    m_insert_when_clicked = ini.GetBool(L"tag_edit", L"insert_when_clicked", true);
    ini.GetStringList(L"tag_edit", L"default_formular", m_default_formular, default_formular);
}

void CTagFromFileNameDlg::InitComboList()
{
    m_format_combo.ResetContent();
    for (const auto& formular : m_default_formular)
    {
        m_format_combo.AddString(formular.c_str());
    }
    m_format_combo.AddString(CCommon::LoadText(IDS_CLEAR_HISTORY));
}

void CTagFromFileNameDlg::InsertTag(const wchar_t* tag)
{
    CString str;
    m_format_combo.GetWindowText(str);
    //CEdit* pEdit = m_format_combo.GetEditCtrl();
    //int start, end;
    //pEdit->GetSel(start, end);
    //pEdit->SetSel(end, end);
    //pEdit->ReplaceSel(tag);
    str += tag;
    m_format_combo.SetWindowText(str);
}

void CTagFromFileNameDlg::SetInsertWhenClicked(bool insert)
{
    if (insert)
        CheckDlgButton(IDC_INSERT_RADIO, TRUE);
    else
        CheckDlgButton(IDC_COPY_RADIO, TRUE);
}

bool CTagFromFileNameDlg::IsInsertWhenClicked() const
{
    return (IsDlgButtonChecked(IDC_INSERT_RADIO) != 0);
}

void CTagFromFileNameDlg::InsertFormular(const wstring& str_formular)
{
    //向m_default_formular中插入一项
    auto iter = std::find(m_default_formular.begin(), m_default_formular.end(), str_formular);
    if (iter == m_default_formular.begin())     //如果要插入的就在第一项，则直接返回
        return;

    if (iter != m_default_formular.end())       //如果列表中包含要插入的项，就将其和第一项交换
    {
        std::swap(*iter, *m_default_formular.begin());
    }
    else        //列表中没有包含要插入的项，就将其插入到最前面
    {
        m_default_formular.insert(m_default_formular.begin(), str_formular);
    }
}

void CTagFromFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_format_combo);
}


BEGIN_MESSAGE_MAP(CTagFromFileNameDlg, CDialog)
    ON_BN_CLICKED(IDC_TITLE_BUTTON, &CTagFromFileNameDlg::OnBnClickedTitleButton)
    ON_BN_CLICKED(IDC_ARTIST_BUTTON, &CTagFromFileNameDlg::OnBnClickedArtistButton)
    ON_BN_CLICKED(IDC_ALBUM_BUTTON, &CTagFromFileNameDlg::OnBnClickedAlbumButton)
    ON_BN_CLICKED(IDC_TRACK_BUTTON, &CTagFromFileNameDlg::OnBnClickedTrackButton)
    ON_BN_CLICKED(IDC_YEAR_BUTTON, &CTagFromFileNameDlg::OnBnClickedYearButton)
    ON_BN_CLICKED(IDC_GENRE_BUTTON, &CTagFromFileNameDlg::OnBnClickedGenreButton)
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(IDC_COMBO1, &CTagFromFileNameDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_COMMENT_BUTTON, &CTagFromFileNameDlg::OnBnClickedCommentButton)
END_MESSAGE_MAP()


// CTagFromFileNameDlg 消息处理程序


BOOL CTagFromFileNameDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    LoadConfig();

    if (!m_dlg_title.IsEmpty())
    {
        SetWindowText(m_dlg_title);
    }

    SetInsertWhenClicked(m_insert_when_clicked);

    SetDlgItemText(IDC_TITLE_BUTTON, FORMULAR_TITLE);
    SetDlgItemText(IDC_ARTIST_BUTTON, FORMULAR_ARTIST);
    SetDlgItemText(IDC_ALBUM_BUTTON, FORMULAR_ALBUM);
    SetDlgItemText(IDC_TRACK_BUTTON, FORMULAR_TRACK);
    SetDlgItemText(IDC_GENRE_BUTTON, FORMULAR_GENRE);
    SetDlgItemText(IDC_YEAR_BUTTON, FORMULAR_YEAR);
    SetDlgItemText(IDC_COMMENT_BUTTON, FORMULAR_COMMENT);

    InitComboList();
    if (!m_default_formular.empty())
        m_format_combo.SetWindowText(m_default_formular.front().c_str());

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CTagFromFileNameDlg::OnBnClickedTitleButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_TITLE);
    else
        CCommon::CopyStringToClipboard(FORMULAR_TITLE);
}


void CTagFromFileNameDlg::OnBnClickedArtistButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_ARTIST);
    else
        CCommon::CopyStringToClipboard(FORMULAR_ARTIST);
}


void CTagFromFileNameDlg::OnBnClickedAlbumButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_ALBUM);
    else
        CCommon::CopyStringToClipboard(FORMULAR_ALBUM);
}


void CTagFromFileNameDlg::OnBnClickedTrackButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_TRACK);
    else
        CCommon::CopyStringToClipboard(FORMULAR_TRACK);
}


void CTagFromFileNameDlg::OnBnClickedYearButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_YEAR);
    else
        CCommon::CopyStringToClipboard(FORMULAR_YEAR);
}


void CTagFromFileNameDlg::OnBnClickedGenreButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_GENRE);
    else
        CCommon::CopyStringToClipboard(FORMULAR_GENRE);
}


void CTagFromFileNameDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    CString str;
    m_format_combo.GetWindowText(str);
    m_formular_selected = str.GetString();
    SaveConfig();
}


void CTagFromFileNameDlg::OnCbnSelchangeCombo1()
{
    // TODO: 在此添加控件通知处理程序代码
    int cur_sel = m_format_combo.GetCurSel();
    if (cur_sel == m_format_combo.GetCount() - 1)
    {
        if (MessageBox(CCommon::LoadText(IDS_CLEAR_HISTORY_INFO), NULL, MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
        {
            m_default_formular = default_formular;
            InitComboList();
        }
        m_format_combo.SetCurSel(0);
    }
    else
    {
        CString str;
        m_format_combo.GetWindowText(str);
        InsertFormular(str.GetString());
    }
}


void CTagFromFileNameDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    CString str;
    m_format_combo.GetWindowText(str);
    InsertFormular(str.GetString());

    CDialog::OnOK();
}


void CTagFromFileNameDlg::OnBnClickedCommentButton()
{
    if (IsInsertWhenClicked())
        InsertTag(FORMULAR_COMMENT);
    else
        CCommon::CopyStringToClipboard(FORMULAR_COMMENT);
}