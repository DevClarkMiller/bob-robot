import js from '@eslint/js';
import globals from 'globals';
import tseslint, { parser } from 'typescript-eslint';
import pluginReact from 'eslint-plugin-react';
import pluginReactHooks from 'eslint-plugin-react-hooks';
import prettierPlugin from 'eslint-plugin-prettier';
import prettier from 'eslint-config-prettier';
import { defineConfig } from 'eslint/config';

export default defineConfig([
	js.configs.recommended,
	{
		files: ['**/*.{js,mjs,cjs,ts,mts,cts,jsx,tsx}'],
		ignores: ["src/dist/**", "src/generated/*.js", "vite.config.ts"],
		languageOptions: {
			parser: tseslint.parser,
			ecmaVersion: 'latest',
			sourceType: 'module',
			globals: { ...globals.browser, ...globals.node },
			parserOptions: {
				ecmaFeatures: { jsx: true },
				projectService: true,
			},
		},
		plugins: {
			prettier: prettierPlugin,
			react: pluginReact,
			'react-hooks': pluginReactHooks,
			'@typescript-eslint': tseslint.plugin,
		},
		settings: {
			react: { version: 'detect' },
		},
		rules: {
			'no-unused-vars': 'off',
			// Prettier formatting
			'prettier/prettier': 'error',
			// TypeScript-specific rules
			'@typescript-eslint/no-unused-vars': ['error', { argsIgnorePattern: '^_', varsIgnorePattern: '^_' }],
			'@typescript-eslint/no-var-requires': 'error',

			// React rules
			'react/prop-types': 'off',
			'react/display-name': 'off',
			'react/react-in-jsx-scope': 'off',

			// React Hooks
			'react-hooks/rules-of-hooks': 'error',
			'react-hooks/exhaustive-deps': 'error',

			// General JS rules
			semi: ['error', 'always'],
			'no-console': 'off',
			'no-multiple-empty-lines': ['error', { max: 1, maxBOF: 0, maxEOF: 0 }],
			'no-useless-concat': 'error',
			'no-array-constructor': 'error',
			'no-new-object': 'error',
			'no-var': 'error',

			// Disable ESLint built-in indent since Prettier handles it
			indent: 'off',

			// JSX handler naming convention
			'react/jsx-handler-names': ['error', { eventHandlerPrefix: 'handle', eventHandlerPropPrefix: 'on' }],
		},
	},
	prettier,
]);
