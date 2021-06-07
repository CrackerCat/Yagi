#ifndef __YAGI_SYMBOLINFO__
#define __YAGI_SYMBOLINFO__

#include <optional>
#include <tuple>
#include <string>
#include <memory>

namespace yagi 
{

	class SymbolInfo
	{
	protected:
		/*!
		 *	\brief	the address of symbol
		 */
		uint64_t m_ea;

		/*!
		 *	\brief	the associate string of the symbol
		 */
		std::string m_name;


	public:

		/*!
		 * \brief	Use to mark specific symbol as imported 
		 */
		static const std::string IMPORT_PREFIX;

		/*!
		 *	\brief	ctor
		 *			prefer using factory load of find
		 */
		explicit SymbolInfo(uint64_t ea, std::string name);

		/*!
		 *	\brief	Copy is authorized because we only use copyable type
		 */
		SymbolInfo(const SymbolInfo&) = default;
		SymbolInfo& operator=(const SymbolInfo&) = default;

		/*!
		 *	\brief	moving is allowed
		 */
		SymbolInfo(SymbolInfo&&) noexcept = default;
		SymbolInfo& operator=(SymbolInfo&&) noexcept = default;

		/*!
		 *	\brief	destructor
		 */
		virtual ~SymbolInfo() = default;

		/*!
		 *	\brief	Try to determine the type (for ghidra) of the name symbol
		 */
		enum class Type
		{
			Function,	// Name is a function
			Label,		// Name is a code label
			Import,		// Name is an import function name
			Other		// Name is other
		};


		/*!
		 *	\brief	getter of the symbol address
		 *	\return	the address of the symbol
		 */
		uint64_t getAddress() const noexcept;

		/*!
		 *	\brief	if symbol refer to a function compute the size of the symbol
		 *	\return	the size of the symbol
		 *	\raise	SymbolIsNotAFunction
		 */
		virtual uint64_t getFunctionSize() const = 0;

		/*!
		 *	\brief	return the guess type of the function
		 *	\return the guessing type
		 */
		Type getType() const noexcept;

		/*!
		 *	\brief	the associate symbol name
		 *	\return symbol string
		 */
		virtual std::string getName() const;

		/*!
		 *	\brief	state of symbol
		 *	\return	true if the symbol is a function
		 */
		virtual bool isFunction() const noexcept = 0;

		/*!
		 *	\brief	state of the function
		 *	\return	true if symbol is associated to a symbol
		 */
		virtual bool isLabel() const noexcept = 0;

		/*!
		 *	\brief	state of the symbol
		 *	\return	true the symbol is associate to an import
		 */
		virtual bool isImport() const noexcept = 0;

		/*!
		 *	\brief	Is the symbol is in Read only mode
		 *			Use to espand static data from read only memory space
		 */
		virtual bool isReadOnly() const noexcept = 0;
	};

	/*!
	 * \brief	A factory interface, to abstract build of symbol 
	 */
	class SymbolInfoFactory {
	public:
		/*!
		 * \brief	default destructor 
		 */
		virtual ~SymbolInfoFactory() = default;

		/*!
		 * \brief	Find any symbol at a particular address
		 * \param	ea	the address of the symbol
		 * \return	optional symbol if found into database
		 */
		virtual std::optional<std::unique_ptr<SymbolInfo>> find(uint64_t ea) = 0;

		/*!
		 * \brief	Find a function symbol from an address anywhere in the function
		 * \param	ea	any address that is handle by a function
		 */
		virtual std::optional<std::unique_ptr<SymbolInfo>> find_function(uint64_t ea) = 0;
	};

}

#endif