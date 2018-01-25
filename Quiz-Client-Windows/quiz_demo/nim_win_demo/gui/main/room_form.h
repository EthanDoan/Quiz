#pragma once

#include "gui/helper/ui_bitmap_control.h"
#include "api/nim_livestreaming_cpp.h"
#include "shared/ui/msgbox.h"

//ѡ��
#define SELECT_NUM_MAX	3
struct SelectInfo
{
	int32_t id_;
	std::string text_;//��ʾ�ı�
	int32_t sel_num_;//����������Ҫ����ʾѡ������
	SelectInfo()
	{
		sel_num_ = -1;
	}
	//{"sel_id":int, "sel_txt":"string","sel_num":int}
	Json::Value GetJsonValue(bool answer = false)
	{
		Json::Value value;
		value["optionId"] = id_;
		value["content"] = text_;
		if (sel_num_ >= 0 && answer)
		{
			value["selectCount"] = sel_num_;
		}
		return value;
	}
	bool ParseJson(Json::Value value)
	{
		if (value.isObject() && value["content"].isString() && value["optionId"].isInt())
		{
			id_ = value["optionId"].asInt();
			text_ = value["content"].asString();
			sel_num_ = 0;
			if (value["selectCount"].isInt())
			{
				sel_num_ = value["selectCount"].asInt();
			}
			return true;
		}
		return false;
	}
};

//��Ŀ
struct QuestionInfo 
{
	int64_t id_;
	std::string text_;
	std::vector<SelectInfo> select_infos_;
	int32_t key_;
	int32_t order_;
	QuestionInfo()
	{
		id_ = -1;
		key_ = -1;
		order_ = 0;
	}
	//{"id":long, "qustion":"string", "select_info":[select,select,select] }
	Json::Value GetJsonValue(bool answer = false)
	{
		Json::Value value;
		value["questionId"] = id_;
		value["question"] = text_;
		value["order"] = order_;
		if (key_ >= 0 && answer)
		{
			value["rightAnswer"] = key_;
		}
		for (auto it : select_infos_)
		{
			value["options"].append(it.GetJsonValue(answer));
		}
		return value;
	}
	bool ParseJson(Json::Value value)
	{
		if (value.isObject() && value["questionId"].isInt64() && value["question"].isString() && value["options"].isArray() && value["options"].size() > 0)
		{
			id_ = value["questionId"].asInt64();
			text_ = value["question"].asString();
			order_ = value["order"].asInt();
			if (value["rightAnswer"].isInt())
			{
				key_ = value["rightAnswer"].asInt();
			}
			select_infos_.clear();
			int32_t sel_num = value["options"].size();
			for (int32_t i = 0; i < sel_num; i++)
			{
				Json::Value item;
				item = value["options"].get(i, item);
				SelectInfo sel_info;
				if (!sel_info.ParseJson(item))
				{
					return false;
				}
				select_infos_.push_back(sel_info);
			}
			return true;
		}
		return false;
	}
};

typedef std::vector<QuestionInfo> QuestionInfos;

struct RoomInfo
{
	int64_t room_id_;
	int32_t money_;
	std::string token_;
	std::string push_url_;
	QuestionInfos questions_;
	int32_t winner_count_;
	std::vector<std::string> winners_;
	FLOAT bonus_;
	RoomInfo()
	{
		room_id_ = 0;
		money_ = 0;
		winner_count_ = 0;
		bonus_ = 0;
	}
};

typedef enum LiveStepType
{
	kLiveStepInit = 0,
	kLiveStepLive,
	kLiveStepQustion,
	kLiveStepEnd,
};
typedef enum QustionStepType
{
	kQustionStepAnswer,
	kQustionStepAnswerShow,
};
typedef enum DeviceType{
	kDeviceTypeMic = 0,
	kDeviceTypeCamera,
	kDeviceTypeDecklink,
};

std::string GetServerUrl();
class RoomForm : 
	public nim_comp::WindowEx
{
public:
	RoomForm();
	~RoomForm();

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
	* ���ݿؼ����������Զ���ؼ�
	* @param[in] pstrClass �ؼ�����
	* @return Control* �����Ŀؼ���ָ��
	*/
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

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
	bool MenuItemClick(ui::EventArgs* msg);

	bool InitRoomInfo(RoomInfo room_info);

private:
	void DoClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void LsErrTipCb(MsgBoxRet);
private:

	static void VideoFrameCb(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_SAMPLER *pstSampler);
	static void VideoFrameCb2(_HNLSSCHILDSERVICE hNLSSChild, ST_NLSS_VIDEO_SAMPLER *pstSampler);
	void InitLs();
	void LsErrCb(EN_NLSS_ERRCODE errCode);
	void ShowDevice(bool mic, ui::CPoint point);
	void OnPaintFrame();
	void StartDevice(DeviceType type, const std::string &path);

	void SetTitleInfo();
	//��ʾtip��ʾ��
	void ShowTip(const std::wstring &tip = L"", const std::wstring &tipex1 = L"", int32_t time = 0, const std::wstring &tipex2 = L"");
	void DoTipTime();
	void RefreshTime(int32_t time);
	//ֱ��ǰҳ�� kLiveStepInit
	void LiveStartShow();
	//ֱ��������
	void LiveStarting();
	//׼������ҳ kLiveStepLive kQustionStepStart
	void QustionStart();
	//��Ŀ���� kLiveStepQustion kQustionStepAnswer
	void QustionAndAnswer();
	//������������õ����������ʾ
	void ShowAnswerRetGet();
	//��ʾ�� kLiveStepQustion kQustionStepAnswerShow
	void QustionShowAnswer();
	void ShowQustion();
	//�н����
	void ShowResult();

	//ֱ��״̬
	void PostLiveStatus(bool end);
	void PostLiveStatusCb(bool ret, int response_code, const std::string& reply);
	//����
	void PushQustion();
	void PushAnswer();
	void PushResult();
	//push��Ϣ
	void PushMsg(const std::string& msg);
	void PushMsgCb(bool ret, int response_code, const std::string& reply);
	//ֱ����
	void DoLiveStart();
	void LiveStartCb(bool ret);
	//������
	void QuestAnswerRet();
	void QuestAnswerRetCb(bool ret, int response_code, const std::string& reply);

public:
	static const LPCTSTR kClassName;

private:
	RoomInfo room_info_;
	uint32_t cur_qustion_num_;
	nim_livestream::LsSession ls_session_;
	std::string audio_path_;
	std::string video_path_;
	std::string decklink_path_;
	LiveStepType live_step_;
	QustionStepType qustion_step_;

	nbase::WeakCallbackFlag paint_video_timer_;
	ui::BitmapControl* pre_viewer_;
	ui::Label* tip_;
	ui::Box* tip_box_;
	ui::Label* tip1_;
	ui::Label* tip2_;
	ui::Label* time_tip_;
	int32_t tip_time_count_;
	ui::Box* qustion_box_;
	ui::Label* qustion_text_;
	ui::Box* sel_box_[SELECT_NUM_MAX];
	ui::Label* sel_info_[SELECT_NUM_MAX];
	ui::Label* sel_num_[SELECT_NUM_MAX];
	ui::Label* answer_num_;
	ui::Button* btn_step_;

	ui::Box* result_box_;
	ui::Label* ret_num_;
	ui::ListBox* ret_member_;
	ui::Label* member_more_;
	ui::Control* empty_tip_;
	ui::Button* btn_end_;

	ui::Box* device_box_;
	ui::ButtonBox* mic_;
	ui::Control* mic_warning_;
	ui::ButtonBox* camera_;
	ui::Control* camera_warning_;
};
using namespace nbase;
