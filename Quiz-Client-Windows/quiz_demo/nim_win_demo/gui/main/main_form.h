#pragma once

#include "room_form.h"

class MainForm : 
	public nim_comp::WindowEx
{
public:
	MainForm();
	~MainForm();

	/**
	* �麯����ָ���������xml�����ļ���ͼƬ�ز����ڵ�Ŀ¼�����·��
	* @return std::wstring ���ظ�Ŀ¼�����[��װĿ¼]/bin/themes/default��·��
	*/
	virtual std::wstring GetSkinFolder() override;

	//�����麯��
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual UINT GetClassStyle() const override;
	
	/**
	* ������������Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void OnFinalMessage(HWND hWnd);

	/**
	* ���ز�����WM_CLOSE��Ϣ
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
	* @return void	�޷���ֵ
	*/
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	* ����ESC������Ϣ����
	* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void OnEsc(BOOL &bHandled);

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

	/**
	* �������пؼ�������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnClicked(ui::EventArgs* msg);

private:

	void ShowErrTip(const std::wstring &tip);

	void AddMoneyItem(const std::wstring &text);

	void OnStart(int32_t money);

	void QuestMasterInfoCb(bool ret, int response_code, const std::string& reply);

	void CreateRoom(RoomInfo info);

public:
	static const LPCTSTR kClassName;

private:
	ui::Combo* money_cmb_;
	ui::Label* err_tip_;
};

using namespace nbase;
//DISABLE_RUNNABLE_METHOD_REFCOUNT(MainForm);
