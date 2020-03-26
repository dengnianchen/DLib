#include "SegInfo.h"

using namespace Dlib::CvExt;



NS_DLIB_PATTERN_BEGIN namespace _Internal {

	void SegInfo::_getRawSegs (SquareIterator<uchar> iter, std::vector<int>& o_segLengths,
		std::vector<SquareIterator<uchar>>& o_segStarts) {

		// ��¼�׶����Ϊ���ε��������
		iter.moveNext ();
		int lengthOfCurLevel = 1;
		bool curLevel = iter.value (_checkBoundary);
		o_segStarts.push_back (iter);

		// �ӵڶ����㿪ʼ����������ֵ��ǰ����ͬ�����ӳ���ǰ�ֶΣ��������µķֶ�
		while (iter.moveNext ()) {
			bool val = iter.value (_checkBoundary);
			if (val == curLevel) {
				lengthOfCurLevel++;
				continue;
			}
			o_segLengths.push_back (lengthOfCurLevel);
			curLevel = val;
			o_segStarts.push_back (iter);
			lengthOfCurLevel = 1;
		}
		o_segLengths.push_back (lengthOfCurLevel);

	}

	

	void SegInfo::_filterSegs (std::vector<int>& io_segLengths,
		std::vector<SquareIterator<uchar>>& io_segStarts, int lengthThreshold) {

		// ����С��ָ����ֵ�ķֶ�
		std::vector<int> filteredSegLengths;
		std::vector<SquareIterator<uchar>> filteredSegStarts;

		// Ѱ�ҵ�һ�����ȴ�����ֵ�ķֶΣ���Ϊ���
		auto startIndex = -1;
		for (auto i = 0; i < io_segLengths.size (); ++i) {
			if (io_segLengths[i] >= lengthThreshold) {
				startIndex = i;
				break;
			}
		}
		if (startIndex == -1)
			startIndex = 0;
		filteredSegLengths.push_back (io_segLengths[startIndex]);
		filteredSegStarts.push_back (io_segStarts[startIndex]);

		// �������зֶΣ����ֶ�С��ָ����ֵ��������ǰ��ֶΣ������ڣ��ϲ�
		for (auto i = 1; i < io_segLengths.size (); ++i) {
			auto index = (startIndex + i) % io_segLengths.size ();
			if (io_segLengths[index] < lengthThreshold) {
				auto& lastLength = filteredSegLengths[filteredSegLengths.size () - 1];
				lastLength += io_segLengths[index];
				if (i < io_segLengths.size () - 1) {
					lastLength += io_segLengths[(index + 1) % io_segLengths.size ()];
					i++;
				}
			} else {
				filteredSegLengths.push_back (io_segLengths[index]);
				filteredSegStarts.push_back (io_segStarts[index]);
			}
		}

		// ���ֶ���Ϊ�������Ҳ�Ϊ1��������β�ֶ�Ϊͬɫ�ֶΣ���Ҫ���кϲ�
		if (filteredSegLengths.size () > 1 && filteredSegLengths.size () % 2 != 0) {
			filteredSegLengths[0] += filteredSegLengths[filteredSegLengths.size () - 1];
			filteredSegStarts[0] = filteredSegStarts[filteredSegStarts.size () - 1];
			filteredSegLengths.pop_back ();
			filteredSegStarts.pop_back ();
		}

		io_segLengths = filteredSegLengths;
		io_segStarts = filteredSegStarts;

	}

} NS_DLIB_PATTERN_END