/*!
	\file

	CSMLDoc and it's associated classes are used to load and/or parse a markup
	language loosly based on HTML. The results of the parsing yields a series
	of paragraph groups and text blocks each of which include their own set
	of formatting attributes. Although the CSMLDoc collection of classes was
	written to be used with the CSMLStatic class, it was kept seperate in order
	to allow it's use with other MFC extentions.
*/

// HTDoc.h: interface for the CSMLDoc class.
//
//////////////////////////////////////////////////////////////////////

//#ifndef DOXYGEN_SHOULD_SKIP_THIS

#if !defined(AFX_SMLDOC_H__198B6DF4_8CCC_445E_AEAD_82674B31A7F6__INCLUDED_)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define AFX_SMLDOC_H__198B6DF4_8CCC_445E_AEAD_82674B31A7F6__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define JUSTIFY_CENTER	0x0001	//!<	Specifies center justification
#define JUSTIFY_RIGHT	0x0002	//!<	Specifies right justification
#define JUSTIFY_LEFT	0x0004	//!<	Specifies left justification
#define STYLE_BOLD		0x0010	//!<	Specifies text block is bold
#define STYLE_ITALICS	0x0020	//!<	Specifies text block is italics
#define STYLE_UNDERLINE	0x0040	//!<	Specifies text block is underlined

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define JUSTIFY_MASK	(JUSTIFY_CENTER | JUSTIFY_LEFT | JUSTIFY_RIGHT)
#define STYLE_MASK		(STYLE_BOLD | STYLE_ITALICS | STYLE_UNDERLINE)
#endif

/*!
	\brief	Document object.

	The CSMLDoc class represents a document containing text groups and
	text blocks.
*/
class CSMLDoc {
private:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	struct CSMLTagOption {
		CSMLTagOption(CString &param, CString &value);

		CString		m_Param;
		CString		m_Value;
	};

	class CSMLTagOptionList : public CList<CSMLTagOption*, CSMLTagOption*> {
	public:
		virtual ~CSMLTagOptionList();
		void Reset();
		CString *GetParam(LPCTSTR param);
		CString GetParamTypeface(CString &defaultFace);
		DWORD GetParamSize(DWORD defaultSize);
		DWORD GetParamColor(DWORD defaultColor);
		DWORD GetBkgndColor(DWORD defaultColor);
	};

	class CSMLFontInfo {
	public:
		CSMLFontInfo();
		CSMLFontInfo(CSMLFontInfo *info);
		CSMLFontInfo(DWORD size, DWORD style, COLORREF color, CString &face);
		CSMLFontInfo(DWORD size, DWORD style, COLORREF color, LPCSTR face);

		DWORD		m_Size;
		DWORD		m_Style;
		COLORREF	m_Color;
		CString		m_Face;
	};
#endif

public:
	/*!
		\brief	List class for managing font information.
	*/
	typedef CList<CSMLFontInfo*, CSMLFontInfo*> CSMLFontInfoList;


	/*!
		\brief	Group object.

		The CSMLTextGroup represents a group of text objects contained within a document.
	*/
	class CSMLTextGroup {

	public:
		/*!
			\brief	text object

			The CSMLTextBlock represents a text objects contained within a text group.
		*/
		class CSMLTextBlock {
		public:
			/*!
				\brief	Constructor

				\param	text	Pointer to the text for the block
				\param	info	Pointer to the font formatting information for the block.
			*/
			CSMLTextBlock(LPCSTR text, CSMLFontInfo *info);

			/*!
				\brief	Retrieves the typeface used by the text block

				\return	A CString containing the typeface.
			*/
			CString &GetTypeface();

			/*!
				\brief	Retrieves the font point size used by the text block

				\return	The point size of the font used by the text block.
			*/
			DWORD GetPointSize();

			/*!
				\brief	Retrieves the formatting style used by the text block

				\return	The formatting style used by the text block.
			*/
			DWORD GetStyle();

			/*!
				\brief	Retrieves the text color used by the text block

				\return	The text color used by the text block.
			*/
			COLORREF GetColor();

			/*!
				\brief	Retrieves the text managed by the block.

				\return	A string containing the text managed by the block.
			*/
			CString &GetText();

		private:
			CString		m_Text;			//!<	CString object containing the text for the block.
			DWORD		m_Style;		//!<	Formatting styles for the text block.
			DWORD		m_PointSize;	//!<	Point size for the font that will be applied to the text block.
			COLORREF	m_Color;		//!<	Text Color.
			CString		m_Typeface;		//!<	Typeface for the text block.
		};

	private:
		CArray<CSMLTextBlock*, CSMLTextBlock*>	m_Blocks;	//!<	Array of text blocks belonging to the group.

	public:
		/*!
			\brief	Constructor.
		*/
		CSMLTextGroup();

		/*!
			\brief	Destructor.
		*/
		~CSMLTextGroup();

		/*!
			\brief	Adds a text block to the group.

			This method adds an already created texth block to the group. Once added
			the group "owns" the text block and may delete or modify it at any time.
			Once a text block has been added to a group, it should not be added to
			any other group until the owner has relinquished ownership.

			\param	block	Pointer to the text block being added to the group.

			\sa	Empty
		*/
		void Add(CSMLTextBlock *block);

		/*!
			\brief	Removes all text blocks from the group.

			This method will remove all text blocks from the group and optionally
			deleted them upon removal.

			\param	bDelete	TRUE if the blocks should be deleted or FALSE if
					they should be removed without being destroyed.
		*/
		void Empty(BOOL bDelete);

		/*!
			\brief Retrieve the number of text blocks owned by the group.

			\return	The number of text blocks owned by the group.
		*/
		int GetBlockCount();

		/*!
			\brief	Retrieves a text block from the group.

			This method returns a text block specified by the index. Before calling
			this method you should ensure that the block at the requested index exists.

			\param	index	A zero based index of the text block to retrieve.

			\return	The text block associated with the index.
		*/
		CSMLTextBlock *GetBlock(int index);

		/*!
			\brief	Assignment operator used for copying a text group.
		*/
		void operator=(CSMLTextGroup &line);
	};


private:
	CArray<CSMLTextGroup*, CSMLTextGroup*>	m_Blocks;	//!< Array of text blocks.
	COLORREF	m_BkgndColor;	//!<	Background color of the document.
	CString		m_PageTypeface;	//!<	Global typeface of the document.

	/*!
		\brief	Parses a tag.

		Parses a formatting tag from the document stream.

		\param	src			Pointer to the document stream.
		\param	tagText		CString buffer to store the tags text identifier in.
		\param	tagID		DWORD argument to store the numerical tag ID in.
		\param	paramList	An option list object used to store all of the tags options in.

		\return	A pointer to the character immediately AFTER the entire tag.
	*/
	LPCTSTR ParseTag(LPCTSTR src, CString &tagText, DWORD &tagID, CSMLTagOptionList &paramList);

	/*!
		\brief	Adds a text group to the document.

		Adds a text group to the document.

		\param	line	A CSMLTextGroup object to add to the document.
	*/
	void AddGroup(CSMLTextGroup &line);

	/*!
		\brief	Purges (deletes) all text groups and text blocks associated with the document.

		Purges (deletes) all text groups and text blocks associated with the document.
	*/
	void PurgeBlocks();

public:
	/*!
		Destructor
	*/
	~CSMLDoc();

	/*!
		\brief	Returns the number of text groups in the document.

		Returns the number of text groups in the document.

		\return	Number of text groups in the document.
	*/
	int GetGroupCount();

	/*!
		\brief	Retrieve text group.

		Retrieves the text group.

		\param	index	Index of the text group to retrieve.
		\return	A Pointer to the text group.
	*/
	CSMLTextGroup *GetTextGroup(int index);

	/*!
		\brief	Parses a "hypertext" stream.

		Parses a "hypertext" stream.

		\param	stream	A null terminated character buffer containing the
						"hypertext" stream to parse.

		\return	State of the parsing operation
		\retval	TRUE	Operation was successful.
		\retval FALSE	Operation failed.
	*/
	BOOL ParseSMLText(LPCTSTR stream);

	/*!
		\brief	Parses a "hypertext" stream.

		Parses a "hypertext" stream.

		\param	nID	ID of the resource containing the "hypertext" stream to parse.
					The resource <b>MUST</b> be a custom type of "SMLDOC".

		\return	State of the parsing
		\retval	TRUE	Operation was successful.
		\retval FALSE	Operation failed.
	*/
	BOOL ParseSMLText(UINT nID);

	/*!
		\brief	Retrieves the background color of the document.

		Retrieves the background color of the document.

		\return	Value represending the background color of the document.
	*/
	COLORREF GetBkgndColor();

	/*!
		\brief	Retrieves the global typeface of the document.

		Retrieves the global typeface of the document.

		\return	A reference to a CString object containing the 
	*/
	CString &GetTypeface();
};



#endif // !defined(AFX_SMLDOC_H__198B6DF4_8CCC_445E_AEAD_82674B31A7F6__INCLUDED_)

